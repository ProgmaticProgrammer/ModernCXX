// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#include "CommandManagerTest.h"
#include "src/utilities/Command.hpp"
#include "src/backend/CommandManager.hpp"

#include <memory>
#include <string>
#include <iostream>

using namespace calculator::controller;
using namespace calculator::utility;
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

namespace {

class TestCommand : public Command
{
public:
    TestCommand(): executeCount_{0} , undoCount_{0} {}

    unsigned int getExecuteCount() const { return executeCount_; }
    unsigned int getUndoCount() const { return undoCount_; }

private:
    void executeImpl() noexcept override { ++executeCount_; }
    void undoImpl() noexcept override { ++undoCount_; }
    unique_ptr<Command> cloneImpl() const noexcept override { return nullptr; }
    const char* helpMessageImpl() const noexcept override { return ""; }

private:
    unsigned int executeCount_;
    unsigned int undoCount_;
};


class TestDeleteCommand : public Command
{
public:
    TestDeleteCommand(bool& b): deleted_{b} { deleted_ = false; }
    ~TestDeleteCommand(){ deleted_ = true; }

private:
    void executeImpl() noexcept override { }
    void undoImpl() noexcept override { }
    unique_ptr<Command> cloneImpl() const noexcept override { return nullptr; }
    const char* helpMessageImpl() const noexcept override { return "TestDeleteCommand for test purpose."; }

private:
    bool& deleted_;
};

}

void CommandManagerTest::testExecute(UndoRedoStrategy st)
{
    TestCommand* raw1 = new TestCommand;
    TestCommand* raw2 = new TestCommand;
    TestCommand* raw3 = new TestCommand;

    CommandManager cm(st);
    QVERIFY( cm.getUndoSize() == 0 );
    QVERIFY( cm.getRedoSize() == 0 );

    cm.executeCommand( MakeCommandPtr(raw1) );
    QVERIFY( cm.getUndoSize() == 1 );

    cm.executeCommand( MakeCommandPtr(raw2) );
    QVERIFY( cm.getUndoSize() == 2 );

    cm.executeCommand( MakeCommandPtr(raw3) );
    QVERIFY( cm.getUndoSize() == 3 );
    QVERIFY( cm.getRedoSize() == 0 );

    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw2->getExecuteCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 0u );
    QCOMPARE( raw2->getUndoCount(), 0u );
    QCOMPARE( raw3->getUndoCount(), 0u );

}

void CommandManagerTest::testUndo(UndoRedoStrategy st)
{
    TestCommand* raw1 = new TestCommand;
    TestCommand* raw2 = new TestCommand;
    TestCommand* raw3 = new TestCommand;

    CommandManager cm(st);

    cm.executeCommand( MakeCommandPtr(raw1) );
    cm.executeCommand( MakeCommandPtr(raw2) );
    cm.executeCommand( MakeCommandPtr(raw3) );
    QVERIFY( cm.getUndoSize() == 3 );

    cm.undo();

    QVERIFY( cm.getUndoSize() == 2 );
    QVERIFY( cm.getRedoSize() == 1 );
    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw2->getExecuteCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 0u );
    QCOMPARE( raw2->getUndoCount(), 0u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    cm.undo();

    QVERIFY( cm.getUndoSize() == 1 );
    QVERIFY( cm.getRedoSize() == 2 );
    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw2->getExecuteCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 0u );
    QCOMPARE( raw2->getUndoCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    cm.undo();

    QVERIFY( cm.getUndoSize() == 0 );
    QVERIFY( cm.getRedoSize() == 3 );
    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw2->getExecuteCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 1u );
    QCOMPARE( raw2->getUndoCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    return;
}

void CommandManagerTest::testRedo(UndoRedoStrategy st)
{
    TestCommand* raw1 = new TestCommand;
    TestCommand* raw2 = new TestCommand;
    TestCommand* raw3 = new TestCommand;

    CommandManager cm(st);

    cm.executeCommand( MakeCommandPtr(raw1) );
    cm.executeCommand( MakeCommandPtr(raw2) );
    cm.executeCommand( MakeCommandPtr(raw3) );
    QVERIFY( cm.getUndoSize() == 3 );

    cm.undo();
    cm.undo();
    cm.undo();

    cm.redo();
    QVERIFY( cm.getUndoSize() == 1 );
    QVERIFY( cm.getRedoSize() == 2 );
    QCOMPARE( raw1->getExecuteCount(), 2u );
    QCOMPARE( raw2->getExecuteCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 1u );
    QCOMPARE( raw2->getUndoCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    cm.redo();
    QVERIFY( cm.getUndoSize() == 2 );
    QVERIFY( cm.getRedoSize() == 1 );
    QCOMPARE( raw1->getExecuteCount(), 2u );
    QCOMPARE( raw2->getExecuteCount(), 2u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 1u );
    QCOMPARE( raw2->getUndoCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    cm.redo();
    QVERIFY( cm.getUndoSize() == 3 );
    QVERIFY( cm.getRedoSize() == 0 );
    QCOMPARE( raw1->getExecuteCount(), 2u );
    QCOMPARE( raw2->getExecuteCount(), 2u );
    QCOMPARE( raw3->getExecuteCount(), 2u );
    QCOMPARE( raw1->getUndoCount(), 1u );
    QCOMPARE( raw2->getUndoCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    return;
}

void CommandManagerTest::testRedoStackFlush(UndoRedoStrategy st)
{
    TestCommand* raw1 = new TestCommand;
    TestCommand* raw2 = new TestCommand;

    CommandManager cm(st);

    cm.executeCommand( MakeCommandPtr(raw1) );
    cm.executeCommand( MakeCommandPtr(raw2) );

    bool deleted = false;

    cm.executeCommand( MakeCommandPtr<TestDeleteCommand>(deleted) );

    // push TestDeleteCommand and raw2 to redo stack
    cm.undo();
    cm.undo();
    QVERIFY( cm.getUndoSize() == 1 );
    QVERIFY( cm.getRedoSize() == 2 );

    // add a new command, which should destroy the redo stack
    TestCommand* raw3 = new TestCommand;
    cm.executeCommand( MakeCommandPtr(raw3) );

    QCOMPARE( deleted, true );
    QVERIFY( cm.getUndoSize() == 2 );
    QVERIFY( cm.getRedoSize() == 0 );

    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 0u );
    QCOMPARE( raw3->getExecuteCount(), 1u );
    QCOMPARE( raw3->getUndoCount(), 0u );

    cm.undo();
    cm.redo();

    QVERIFY( cm.getUndoSize() == 2 );
    QVERIFY( cm.getRedoSize() == 0 );

    QCOMPARE( raw1->getExecuteCount(), 1u );
    QCOMPARE( raw1->getUndoCount(), 0u );
    QCOMPARE( raw3->getExecuteCount(), 2u );
    QCOMPARE( raw3->getUndoCount(), 1u );

    cm.undo();
    cm.undo();
    cm.redo();
    cm.redo();
    cm.undo();

    QVERIFY( cm.getUndoSize() == 1 );
    QVERIFY( cm.getRedoSize() == 1 );

    QCOMPARE( raw1->getExecuteCount(), 2u );
    QCOMPARE( raw1->getUndoCount(), 1u );
    QCOMPARE( raw3->getExecuteCount(), 3u );
    QCOMPARE( raw3->getUndoCount(), 3u );

    return;
}

void CommandManagerTest::testResourceCleanup(UndoRedoStrategy st)
{
    bool deleted1 = false;
    bool deleted2 = false;
    bool deleted3 = false;

    CommandManager* cm = new CommandManager(st);
    cm->executeCommand( MakeCommandPtr<TestDeleteCommand>(deleted1) );
    cm->executeCommand( MakeCommandPtr<TestDeleteCommand>(deleted2) );
    cm->executeCommand( MakeCommandPtr<TestDeleteCommand>(deleted3) );

    QCOMPARE( deleted1, false );
    QCOMPARE( deleted2, false );
    QCOMPARE( deleted3, false );

    delete cm;

    QCOMPARE( deleted1, true );
    QCOMPARE( deleted2, true );
    QCOMPARE( deleted3, true );

    return;
}

void CommandManagerTest::ignoreError(UndoRedoStrategy st)
{
    // This test passes by simply not crashingr
    CommandManager cm(st);
    cm.undo();
    cm.redo();

    return;
}

void CommandManagerTest::testExecuteStackStrategy()
{
    testExecute(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::testUndoStackStrategy()
{
    testUndo(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::testRedoStackStrategy()
{
    testRedo(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::testRedoStackFlushStackStrategy()
{
    testRedoStackFlush(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::testResourceCleanupStackStrategy()
{
    testResourceCleanup(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::ignoreErrorStackStrategy()
{
    ignoreError(UndoRedoStrategy::StackStrategy);
}

void CommandManagerTest::testExecuteListStrategy()
{
    testExecute(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::testUndoListStrategy()
{
    testUndo(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::testRedoListStrategy()
{
    testRedo(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::testRedoStackFlushListStrategy()
{
    testRedoStackFlush(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::testResourceCleanupListStrategy()
{
    testResourceCleanup(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::ignoreErrorListStrategy()
{
    ignoreError(UndoRedoStrategy::ListStrategy);
}

void CommandManagerTest::testExecuteListStrategyVector()
{
    testExecute(UndoRedoStrategy::ListStrategyVector);
}

void CommandManagerTest::testUndoListStrategyVector()
{
    testUndo(UndoRedoStrategy::ListStrategyVector);
}

void CommandManagerTest::testRedoListStrategyVector()
{
    testRedo(UndoRedoStrategy::ListStrategyVector);
}

void CommandManagerTest::testRedoStackFlushListStrategyVector()
{
    testRedoStackFlush(UndoRedoStrategy::ListStrategyVector);
}

void CommandManagerTest::testResourceCleanupListStrategyVector()
{
    testResourceCleanup(UndoRedoStrategy::ListStrategyVector);
}

void CommandManagerTest::ignoreErrorListStrategyVector()
{
    ignoreError(UndoRedoStrategy::ListStrategyVector);
}

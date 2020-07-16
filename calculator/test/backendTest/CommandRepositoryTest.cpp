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

#include <sstream>
#include <memory>
#include "CommandRepositoryTest.h"
#include "src/backend/CommandRepository.hpp"
#include "src/utilities/Command.hpp"
#include "src/backend/CommandRepository.hpp"
#include "src/utilities/Exception.hpp"

using std::make_unique;
using std::unique_ptr;
using std::ostringstream;
using std::set;
using std::string;
using namespace calculator::utility;
using namespace calculator::controller;


class TestCommand : public Command
{
public:
    explicit TestCommand(string optionalName): Command{}, optionalName_{std::move(optionalName)}{}
    TestCommand(const TestCommand&)=default;
    ~TestCommand()=default;
    const string& getOptionalName() const { return optionalName_; }

private:
    void executeImpl() noexcept override { }
    void undoImpl() noexcept override { }
    CommandPtr cloneImpl() const override { return make_unique<TestCommand>(*this); }
    const char* helpMessageImpl() const noexcept override { return "Test command, do nothing."; }

private:
    string optionalName_;
};


void CommandRepositoryTest::testRegister()
{
    CommandRepository cr;
    QVERIFY( cr.size() == 0 );

    string name1 = "command 1";
    string name2 = "command 2";
    set<string> names = { name1, name2 };

    cr.registerCmd( name1, MakeCommandPtr<TestCommand>("TestCmd1") );
    cr.registerCmd( name2, MakeCommandPtr<TestCommand>("TestCmd2") );

    QVERIFY( cr.size() == 2 );

    set<string> registeredCommands = cr.allCmdNames();

    QCOMPARE( registeredCommands, names );

    QCOMPARE( cr.contains(name1), true );
    QCOMPARE( cr.contains(name2), true );
    QCOMPARE( cr.contains("not present"), false );

}

void CommandRepositoryTest::testDuplicateRegister()
{
    CommandRepository cr;

    string name = "command";

    cr.registerCmd( name, MakeCommandPtr<TestCommand>("TestCmd1") );

    QVERIFY( cr.size() == 1 );

    try
    {
        cr.registerCmd( name, MakeCommandPtr<TestCommand>("") );
        QVERIFY( false );
    }
    catch(Exception& e)
    {
        ostringstream oss;
        oss << "Command " << name << " already registered";
        QCOMPARE( e.what(), oss.str() );
    }

    QVERIFY( cr.size() == 1 );

}

void CommandRepositoryTest::testDeregister()
{
    CommandRepository cr;
    cr.clear();

    string name1 = "command 1";
    string name2 = "command 2";

    cr.registerCmd( name1, MakeCommandPtr<TestCommand>("") );
    cr.registerCmd( name2, MakeCommandPtr<TestCommand>("") );

    QVERIFY( cr.size() == 2 );

    QVERIFY(cr.unregisterCmd( name1 ) == 1);

    QVERIFY( cr.size() == 1 );
    QCOMPARE( cr.contains(name1), false );
    QCOMPARE( cr.contains(name2), true );

    QVERIFY(cr.unregisterCmd( "not present" ) == 0);

    QVERIFY( cr.size() == 1 );
    QCOMPARE( cr.contains(name1), false );
    QCOMPARE( cr.contains(name2), true );

}

void CommandRepositoryTest::testAllocateCommand()
{
    CommandRepository cv;
    cv.clear();

    string name = "command";

    cv.registerCmd( name, MakeCommandPtr<TestCommand>(name) );

    QVERIFY( cv.size() == 1 );
    QCOMPARE( cv.contains(name), true );

    CommandPtr clone = cv.clone(name);

    QVERIFY( cv.size() == 1 );
    QCOMPARE( cv.contains(name), true );
    QVERIFY( clone != nullptr );
    TestCommand* testCommand = dynamic_cast<TestCommand*>( clone.get() );
    QVERIFY( testCommand != nullptr );
    QCOMPARE( testCommand->getOptionalName(), name );


    clone = cv.clone("not present");
    QVERIFY( clone == nullptr );

}

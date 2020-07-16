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

#ifndef COMMAND_MANAGER_TEST_H
#define COMMAND_MANAGER_TEST_H

#include <QtTest/QtTest>
#include "backend/CommandManager.hpp"

class CommandManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void testExecuteStackStrategy();
    void testUndoStackStrategy();
    void testRedoStackStrategy();
    void testRedoStackFlushStackStrategy();
    void testResourceCleanupStackStrategy();
    void ignoreErrorStackStrategy();

    void testExecuteListStrategy();
    void testUndoListStrategy();
    void testRedoListStrategy();
    void testRedoStackFlushListStrategy();
    void testResourceCleanupListStrategy();
    void ignoreErrorListStrategy();

    void testExecuteListStrategyVector();
    void testUndoListStrategyVector();
    void testRedoListStrategyVector();
    void testRedoStackFlushListStrategyVector();
    void testResourceCleanupListStrategyVector();
    void ignoreErrorListStrategyVector();

private:
    using UndoRedoStrategy = calculator::controller::CommandManager::UndoRedoStrategy;
    void testExecute(UndoRedoStrategy);
    void testUndo(UndoRedoStrategy);
    void testRedo(UndoRedoStrategy);
    void testRedoStackFlush(UndoRedoStrategy);
    void testResourceCleanup(UndoRedoStrategy);
    void ignoreError(UndoRedoStrategy);
};

#endif

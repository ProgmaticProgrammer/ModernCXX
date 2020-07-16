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

#include "CommandDispatcherTest.h"
#include "src/frontend/UserInterface.hpp"
#include "src/backend/CalcCommand.hpp"
#include "src/backend/CommandDispatcher.hpp"
#include "src/backend/CommandRepository.hpp"
#include "src/backend/CalcModel.hpp"

#include <cmath>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::ostringstream;
using std::string;
using namespace calculator::view;
using namespace calculator::model;
using namespace calculator::controller;

namespace {

class TestInterface : public UserInterface
{
public:
    TestInterface() : lastMessage_{""} { }
    void postMessage(const string& m) override { lastMessage_ = m; }
    void modelChanged() override { }
    const string& getLastMessage() const { return lastMessage_; }

    double top() const {return 0.0;}

private:
    string lastMessage_;
};

}

void CommandDispatcherTest::testCommandDispatcher()
{
    //CommandRepository cr;
    //CalcModel<> st;
    TestInterface ui;
    CommandDispatcher ce{ui};

    ce.commandEntered("+");
    QCOMPARE(ui.getLastMessage(), string{"Command + is not a known command"});

    ce.RegisterCoreCommands();

    ce.commandEntered("+");
    QCOMPARE(ui.getLastMessage(), string{"Stack must have 2 elements"});

    ce.commandEntered("1.0");
    ce.commandEntered("2.0");

    ce.commandEntered("+");

    QCOMPARE( ce.result(), 3.0 );

    ce.commandEntered("-18.3");
    ce.commandEntered("swap");
    QCOMPARE( ce.result(), 3.0 );

    ce.commandEntered("drop");
    QCOMPARE( ce.result(), -18.3);

    ce.commandEntered("22");
    ce.commandEntered("undo");
    QCOMPARE( ce.result(), -18.3);

    ce.commandEntered("undo");
    QCOMPARE(ce.result(), 3.0);

    ce.commandEntered("redo");
    QCOMPARE(ce.result(), -18.3);

    ce.commandEntered("undo");
    ce.commandEntered("-");
    QCOMPARE(ce.result(), -21.3);

    ce.commandEntered("neg");
    QCOMPARE(ce.result(), 21.3);

    ce.commandEntered("7.1");
    ce.commandEntered("/");
    QCOMPARE(ce.result(), 21.3 / 7.1 );

    ce.commandEntered("6");
    ce.commandEntered("2");
    ce.commandEntered("pow");
    QCOMPARE(ce.result(), 36.0);

//    ce.commandEntered("27");
//    ce.commandEntered("3");
//    ce.commandEntered("root");
//    QCOMPARE(ce.result(), 3.0);

//    ce.commandEntered("*");;
//    QCOMPARE(ce.result(), 108.0);

//    ce.commandEntered("0.8");
//    ce.commandEntered("sin");
//    QCOMPARE(ce.result(), std::sin(0.8));

//    ce.commandEntered("dup");
//    ce.commandEntered("arcsin");
//    ce.commandEntered("swap");
//    double t = ce.result();
//    ce.commandEntered("drop");
//    QVERIFY(std::abs(ce.result() - std::asin(t)) < 1e-10);

//    ce.commandEntered("0.7");
//    ce.commandEntered("cos");
//    QCOMPARE(ce.result(), std::cos(0.7));

//    ce.commandEntered("dup");
//    ce.commandEntered("arccos");
//    ce.commandEntered("swap");
//    t = ce.result();
//    ce.commandEntered("drop");
//    QVERIFY(std::abs(ce.result() - std::acos(t)) < 1e-10);

//    ce.commandEntered("0.3");
//    ce.commandEntered("tan");
//    QCOMPARE(ce.result(), std::tan(0.3));

//    ce.commandEntered("dup");
//    ce.commandEntered("arctan");
//    ce.commandEntered("swap");
//    t = ce.result();
//    ce.commandEntered("drop");
//    QVERIFY(std::abs(ce.result() - std::atan(t)) < 1e-10);

}


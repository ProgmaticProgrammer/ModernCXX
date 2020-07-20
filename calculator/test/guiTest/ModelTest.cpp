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

#include <QtWidgets>
#include <QtTest/QtTest>
#include "model.h"

#include "ModelTest.h"

Model model;

void ModelTest::init()
{
    model.reset();
}

void ModelTest::cleanup()
{}

void ModelTest::testCreated_AtWaitingLhsState()
{
    Model model;
    QCOMPARE(model.state(), Model::State::WAITING_LHS);
}

void ModelTest::testOneInput_LhsAssigned()
{
    model.input_operand(1);
    QCOMPARE(model.lhs(), 1.0);
}

void ModelTest::testOnePlusInput_LhsOpAssigned()
{
    model.input_operand(1);
    model.input_operator(Calculator::Plus);
    QCOMPARE(model.lhs(), 1.0);
    QCOMPARE(model.op(), "+");
}

void ModelTest::testOnePlusTwoInput_ResultAvailable()
{
    model.input_operand(1);
    model.input_operator(Calculator::Plus);
    model.input_operand(2);

    QCOMPARE(model.lhs(), 1.0);
    QCOMPARE(model.op(), "+");
    QCOMPARE(model.rhs(), 2.0);

    double result = 0;
    auto ok = model.get_result(result);
    QCOMPARE(ok, true);
    QCOMPARE(result, 3.0);
}

#include <iostream>
#include <QStringList>
#include <unordered_map>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    // I use QStringList instead of argv directly because the QStringList
    // argument to qExec is constant. API doc indicates that for the argv
    // version, it may alter argv making repeated calls to qExec unstable
    // with respect to the arguments.
    QStringList args;
    for(int i = 0; i < argc; ++i)
        args.append( QString{argv[i]} );

    std::unordered_map<std::string, int> passFail;

    ModelTest git;
    passFail["GuiInterfaceTest"] = QTest::qExec(&git, args);

    cout << endl;
    int errors = 0;
    for(const auto& i : passFail)
    {
        errors += i.second;
        if(i.second != 0)
            cout << "Failed test in " << i.first << endl;
    }

    if(errors == 0)
        cout << "All tests passed" << endl;

    return 0;
}

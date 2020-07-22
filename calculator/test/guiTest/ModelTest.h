// Copyright 2020 Lei Zhang
// Contact: lei.zhang.auckland@gmail.com
//
// This file is part of Calculator.
//
// Calculator is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Calculator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#ifndef MODEL_TEST_H
#define MODEL_TEST_H

#include <QtTest/QtTest>

class ModelTest : public QObject
{
    Q_OBJECT
public:

private slots:
    void init();
    void cleanup();

    // test start at a known state
    void test_AfterStarted_AtEmptyState();

    // test normal inputs (operands+operators) gradually
    void test_AfterOneInput_LhsReady();
    void test_AfterOnePlusInput_WaitingForRhs();
    void test_AfterOnePlusTwoIn_EqIn_ResultAvailable();

    void test_AfterOnePlusTwoInput_UnaryOpInput_UnaryAppliesToRhs();
    void test_AfterOnePlusTwoInput_BinaryOpInput_LhsEqualsResult();

    void test_AfterOnePlusTwoInput_PlusThreeEnter_ResultIsSix();

    // test mix operators
    void test_AfterOnePlusTwoInput_PlusThreeEnter_ResultIsNine();

    // test unary operators
    void test_AfterTwoInput_PowIn_LhsIsFour();
    void test_AfterOnePlusTwoInput_PowIn_LhsIsFour();

    // test multiple enters
    void test_AfterOnePlusTwoInput_EnterTwice_ResultIsFive();
private:

};

#endif

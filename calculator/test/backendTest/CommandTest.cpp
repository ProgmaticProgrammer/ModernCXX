#include "CommandTest.h"
#include "backend/Command.hpp"
#include "backend/Stack.hpp"
#include "StackEventsObserver.h"

using namespace calculator::controller;

void CommandTest::testEnterNumber()
{
    Stack<double> stack;
    double number = 7.3;

    EnterNumber<double> en{number, stack};

    Command& command = en;

    command.execute();

    QVERIFY( stack.size() == 1 );
    QCOMPARE(stack.top(), number );
    //QVERIFY( raw->changeCount() == 1 );

    command.undo();

    QVERIFY( stack.size() == 0 );
    //QCOMPARE( raw->changeCount(), 2u );
}

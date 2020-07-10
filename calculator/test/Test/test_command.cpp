#include <QtTest>
#include "Stack.hpp"
#include "Command.hpp"

using namespace calculator::model;
using namespace calculator::controller;

class CommandTest : public QObject
{
    Q_OBJECT

private slots:
    void testEnterNumber();



};

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

QTEST_APPLESS_MAIN(CommandTest)

#include "test_command.moc"

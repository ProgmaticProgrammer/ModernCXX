#include "CommandTest.h"
#include "backend/Command.hpp"
#include "backend/Stack.hpp"
#include "StackEventsObserver.h"

using namespace calculator::controller;
using namespace calculator::model;
using std::unique_ptr;

using Model = Stack<double>;

std::unique_ptr<Model> stack{nullptr};
std::shared_ptr<StackChangedObserver> observer{nullptr};

void CommandTest::initTestCase()
{}
void CommandTest::cleanupTestCase()
{}

void CommandTest::init()
{
  stack.reset(new Model());
  observer.reset(new StackChangedObserver("StackChangedObserver"));

  stack->attach( Model::StackChanged, observer );
}
void CommandTest::cleanup()
{
  stack->detach(Model::StackChanged, "StackChangedObserver");

  observer.reset();
  stack.reset();
}

// just test that it cloned to the correct type, not that the commands
// are equal
template<typename T>
void testClone(Command& c)
{
    unique_ptr<Command> clone{c.clone()};

    QVERIFY( dynamic_cast<T*>(clone.get()) != nullptr );
}

void pre_check()
{
    QVERIFY( stack );
    QVERIFY( observer );

    QVERIFY( stack->size() == 0 );
}


Model& getModel()
{
    pre_check();
    return *stack;
}

//////////////////////////////

void CommandTest::testEnterNumber()
{

    double number = 7.3;
    EnterNumber<> en{number, *stack};

    Command& command = en;

    command.execute();

    QVERIFY( stack->size() == 1 );
    QCOMPARE(stack->top(), number );
    QVERIFY( observer->changeCount() == 1 );

    command.undo();

    QVERIFY( stack->size() == 0 );
    QCOMPARE( observer->changeCount(), 2u );

}


void CommandTest::testEnterNumberClone()
{
    EnterNumber<> en{1.0, *stack};
    testClone<EnterNumber<>>(en);
}

void CommandTest::testSwapTopOfStackPreconditions()
{
    auto& stack = getModel();
    SwapTopOfStack<> stos(stack);
    Command& command = stos;

    try
    {
        command.execute();// empty
        QVERIFY(false);
    }
    catch(Exception&)
    {
        QVERIFY(true);
    }

    double number1 = 2.2;
    stack.push(number1);
    try
    {
        command.execute();// only one
        QVERIFY(false);
    }
    catch(Exception&)
    {
        QVERIFY(true);
    }

    double number2 = 3.3;
    stack.push(number2);
    try
    {
        command.execute();// two, preconditon met
        QVERIFY(true);
    }
    catch(Exception&)
    {
        QVERIFY(false);
    }
}
void CommandTest::testSwapTopOfStackClone()
{
    auto& stack = getModel();
    SwapTopOfStack<> stos(stack);
    testClone<SwapTopOfStack<>>(stos);
}
void CommandTest::testSwapTopOfStack()
{
    auto& stack = getModel();
    SwapTopOfStack<> stos(stack);
    Command& command = stos;

    double number1 = 2.2;
     stack.push(number1);

     double number2 = 3.3;
     stack.push(number2);

     vector<double> v = stack.copyElements(2);
     QCOMPARE( v[0], number2 );
     QCOMPARE( v[1], number1 );

     command.execute();

     QCOMPARE( observer->changeCount(), 3u );

     v = stack.copyElements(2);
     QCOMPARE( v[0], number1 );
     QCOMPARE( v[1], number2 );

     command.undo();

     QCOMPARE( observer->changeCount(), 4u );

     v = stack.copyElements(2);
     QCOMPARE( v[0], number2 );
     QCOMPARE( v[1], number1 );
}

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
void CommandTest::testEnterNumber()
{

    double number = 7.3;
    EnterNumber<double> en{number, *stack};

    Command& command = en;

    command.execute();

    QVERIFY( stack->size() == 1 );
    QCOMPARE(stack->top(), number );
    QVERIFY( observer->changeCount() == 1 );

    command.undo();

    QVERIFY( stack->size() == 0 );
    QCOMPARE( observer->changeCount(), 2u );

}

// just test that it cloned to the correct type, not that the commands
// are equal
template<typename T>
void testClone(Command& c)
{
    unique_ptr<Command> clone{c.clone()};

    QVERIFY( dynamic_cast<T*>(clone.get()) != nullptr );
}

void CommandTest::testEnterNumberClone()
{
    EnterNumber<double> en{1.0, *stack};
    testClone<EnterNumber<double>>(en);
}

#include "CommandTest.h"
#include "backend/Command.hpp"
#include "backend/Stack.hpp"
#include "StackEventsObserver.h"

using namespace calculator::controller;
using namespace calculator::model;

using Model = Stack<double>;

std::unique_ptr<Model> stack;
std::shared_ptr<StackChangedObserver> observer;

void CommandTest::initTestCase()
{}
void CommandTest::cleanupTestCase()
{}
void CommandTest::init()
{
stack = std::make_unique<Model>();
observer = std::make_shared<StackChangedObserver>("StackChangedObserver");
stack->attach( Model::StackChanged, observer );
}
void CommandTest::cleanup()
{
stack->detach(Model::StackChanged, "StackChangedObserver");
stack.reset();
observer.reset();
}
void CommandTest::testEnterNumber()
{

    double number = 7.3;
    //auto observer = std::make_shared<StackChangedObserver>("StackChangedObserver");
    //stack->attach( Model::StackChanged, shared_ptr<Observer>{observer} );

    EnterNumber<double> en{number, *stack};

    Command& command = en;

    command.execute();

    QVERIFY( stack->size() == 1 );
    QCOMPARE(stack->top(), number );
    QVERIFY( observer->changeCount() == 1 );

    command.undo();

    QVERIFY( stack->size() == 0 );
    QCOMPARE( observer->changeCount(), 2u );
    //stack->detach(Model::StackChanged, "StackChangedObserver");
}
void CommandTest::testEnterNumberClone()
{}

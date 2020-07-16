#include "CommandTest.h"

#include "StackEventsObserver.h"
#include "backend/CalcCommand.hpp"
#include "backend/CalcModel.hpp"

using namespace calculator::controller;
using namespace calculator::model;
using std::unique_ptr;

using Model = CalcModel<double>;

std::unique_ptr<Model> model{nullptr};
std::shared_ptr<StackChangedObserver> observer{nullptr};

void CommandTest::initTestCase() {}
void CommandTest::cleanupTestCase() {}

void CommandTest::init() {
  model.reset(new Model());
  observer.reset(new StackChangedObserver("StackChangedObserver"));

  model->attach(Model::ModelChanged, observer);
}
void CommandTest::cleanup() {
  model->detach(Model::ModelChanged, "StackChangedObserver");

  observer.reset();
  model.reset();
}

// just test that it cloned to the correct type, not that the commands
// are equal
template <typename T>
void testClone(Command& c) {
  unique_ptr<Command> clone{c.clone()};

  QVERIFY(dynamic_cast<T*>(clone.get()) != nullptr);
}

void pre_check() {
  QVERIFY(model);
  QVERIFY(observer);

  QVERIFY(model->size() == 0);
}

Model& getModel() {
  pre_check();
  return *model;
}

//////////////////////////////

void CommandTest::testEnterNumber() {
  double number = 7.3;
  EnterNumber<> en{number, getModel()};

  Command& command = en;

  command.execute();

  QVERIFY(model->size() == 1);
  QCOMPARE(model->top(), number);
  QVERIFY(observer->changeCount() == 1);

  command.undo();

  QVERIFY(model->size() == 0);
  QCOMPARE(observer->changeCount(), 2u);
}

void CommandTest::testEnterNumberClone() {
  EnterNumber<> en{1.0, getModel()};
  testClone<EnterNumber<>>(en);
}

void CommandTest::testSwapTopOfStackPreconditions() {
  auto& model = getModel();
  SwapTopOfStack<> stos(model);
  Command& command = stos;

  try {
    command.execute();  // empty
    QVERIFY(false);
  } catch (Exception&) {
    QVERIFY(true);
  }

  double number1 = 2.2;
  model.push(number1);
  try {
    command.execute();  // only one
    QVERIFY(false);
  } catch (Exception&) {
    QVERIFY(true);
  }

  double number2 = 3.3;
  model.push(number2);
  try {
    command.execute();  // two, preconditon met
    QVERIFY(true);
  } catch (Exception&) {
    QVERIFY(false);
  }
}

void CommandTest::testSwapTopOfStackClone() {
  auto& model = getModel();
  SwapTopOfStack<> stos(model);
  testClone<SwapTopOfStack<>>(stos);
}

void CommandTest::testSwapTopOfStack() {
  auto& model = getModel();
  SwapTopOfStack<> stos(model);
  Command& command = stos;

  double number1 = 2.2;
  model.push(number1);

  double number2 = 3.3;
  model.push(number2);

  vector<double> v = model.copyElements(2);
  QCOMPARE(v[0], number2);
  QCOMPARE(v[1], number1);

  command.execute();

  QCOMPARE(observer->changeCount(), 3u);

  v = model.copyElements(2);
  QCOMPARE(v[0], number1);
  QCOMPARE(v[1], number2);

  command.undo();

  QCOMPARE(observer->changeCount(), 4u);

  v = model.copyElements(2);
  QCOMPARE(v[0], number2);
  QCOMPARE(v[1], number1);
}

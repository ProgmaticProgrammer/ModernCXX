// add necessary includes here
#include <algorithm>
#include "ModelTest.h"
#include "ModelEventsObserver.h"

StackTest::StackTest() {}

StackTest::~StackTest() {}

using Model = CalcModel<double>;

void StackTest::testCtor_isEmpty() {
  Model stack_;
  QVERIFY(stack_.size() == 0);
}

void StackTest::testClear_becomeEmpty() {

  Model stack_;

  auto observer = std::make_shared<StackChangedObserver>("StackChangedObserver");
  stack_.attach( Model::ModelChanged, shared_ptr<Observer>{observer} );

  stack_.push(1.0);
  QCOMPARE(observer->changeCount(), 1u);

  stack_.push(2.0);
  QVERIFY(stack_.size() == 2);
  QCOMPARE(observer->changeCount(), 2u);

  stack_.clear();
  QCOMPARE(observer->changeCount(), 3u);

  QVERIFY(stack_.size() == 0);
  stack_.detach(Model::ModelChanged, "StackChangedObserver");
}

void StackTest::testGetElements_atMostSize() {
  Model stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QCOMPARE(stack_.copyElements(3), std::vector<double>({2.0, 1.0}));
}

void StackTest::testPush_savedInReverseOrder() {
  Model stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements(2) == std::vector<double>{2.0, 1.0}));
}

void StackTest::testPop_onEmptyStack() {
  Model stack_;
  QVERIFY(stack_.size() == 0);
  try
  {
      stack_.pop();
      QVERIFY(false);
  }
  catch(Exception& e)
  {
      QCOMPARE(e.what(), StackEvent::ErrorMessages[ErrorConditions::Empty]);
  }
}

void StackTest::testPop_oneAtATime() {
  Model stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements() == std::vector<double>{2.0, 1.0}));
  stack_.pop();
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0}));
  stack_.pop();
  QVERIFY((stack_.copyElements() == std::vector<double>{}));
}

void StackTest::testSwapTop_whenAtLessTwo() {
  Model stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements() == std::vector<double>{2.0, 1.0}));
  stack_.swapTop2();
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0, 2.0}));
}

void StackTest::testSwapTop_whenLessThanTwo() {
  Model stack_;
  stack_.push(1.0);
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0}));
  try
  {
      stack_.swapTop2();
      QVERIFY(false);
  }
  catch(Exception& e)
  {
      QCOMPARE(e.what(), StackEvent::ErrorMessages[ErrorConditions::TooFewArguments]);
  }
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0}));
}

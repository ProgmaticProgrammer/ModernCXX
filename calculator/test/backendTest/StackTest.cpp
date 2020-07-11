// add necessary includes here
#include <algorithm>
#include "StackTest.h"
#include "StackEventsObserver.h"

StackTest::StackTest() {}

StackTest::~StackTest() {}

void StackTest::testCtor_isEmpty() {
  Stack<double> stack_;
  QVERIFY(stack_.size() == 0);
}

void StackTest::testClear_becomeEmpty() {
  Stack<double> stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY(stack_.size() == 2);
  stack_.clear();
  QVERIFY(stack_.size() == 0);
}

void StackTest::testGetElements_atMostSize() {
  Stack<double> stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QCOMPARE(stack_.copyElements(3), std::vector<double>({2.0, 1.0}));
}

void StackTest::testPush_savedInReverseOrder() {
  Stack<double> stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements(2) == std::vector<double>{2.0, 1.0}));
}

void StackTest::testPop_onEmptyStack() {
  Stack<double> stack_;
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
  Stack<double> stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements() == std::vector<double>{2.0, 1.0}));
  stack_.pop();
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0}));
  stack_.pop();
  QVERIFY((stack_.copyElements() == std::vector<double>{}));
}

void StackTest::testSwapTop_whenAtLessTwo() {
  Stack<double> stack_;
  stack_.push(1.0);
  stack_.push(2.0);
  QVERIFY((stack_.copyElements() == std::vector<double>{2.0, 1.0}));
  stack_.swapTop2();
  QVERIFY((stack_.copyElements() == std::vector<double>{1.0, 2.0}));
}

void StackTest::testSwapTop_whenLessThanTwo() {
  Stack<double> stack_;
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

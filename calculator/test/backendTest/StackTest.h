#ifndef STACK_TEST_HEADER
#define STACK_TEST_HEADER

#include <QtTest>

class StackTest : public QObject {
  Q_OBJECT

 public:
  StackTest();
  ~StackTest();

 private slots:
  void testCtor_isEmpty();
  void testClear_becomeEmpty();
  void testGetElements_atMostSize();
  void testPush_savedInReverseOrder();
  void testPop_onEmptyStack();
  void testPop_oneAtATime();
  void testSwapTop_whenAtLessTwo();
  void testSwapTop_whenLessThanTwo();

 private:
  // Stack<double> stack_;
};

#endif
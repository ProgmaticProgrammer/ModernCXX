#include <unity.h>

#include <algorithm>
#include <memory>
#include <type_traits>

#include "calculator.h"
/////////////////////////
//    test cases       //
/////////////////////////
void test_BigCalculatorInvoked(void) {
  auto const input = Input{50};

  auto const calculators = Calculator::getCalculators();
  auto const calculator = std::find_if(
      begin(calculators), end(calculators),
      [&input](auto&& calculator) { return calculator.handles(input); });
  TEST_ASSERT(calculator != end(calculators));
  TEST_ASSERT((*calculator).handles ==
              std::addressof(Calculator::BigCalculator::handles));
  TEST_ASSERT((*calculator).compute ==
              std::addressof(Calculator::BigCalculator::compute));
  TEST_ASSERT((*calculator).log ==
              std::addressof(Calculator::BigCalculator::log));
  if (calculator != end(calculators)) {
    auto const output = calculator->compute(input);
    calculator->log(input, output);
  }
}
void test_SmallCalculatorInvoked(void) {
  auto const input = Input{5};

  auto const calculators = Calculator::getCalculators();
  auto const calculator = std::find_if(
      begin(calculators), end(calculators),
      [&input](auto&& calculator) { return calculator.handles(input); });
  TEST_ASSERT(calculator != end(calculators));
  TEST_ASSERT((*calculator).handles ==
              std::addressof(Calculator::SmallCalculator::handles));
  TEST_ASSERT((*calculator).compute ==
              std::addressof(Calculator::SmallCalculator::compute));
  TEST_ASSERT((*calculator).log ==
              std::addressof(Calculator::SmallCalculator::log));
  if (calculator != end(calculators)) {
    auto const output = calculator->compute(input);
    calculator->log(input, output);
  }
}
/////////////////////////
//  Setup and register //
/////////////////////////
// call for each test
void setUp(void) {}
// call for each test
void tearDown(void) {}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_BigCalculatorInvoked);
  RUN_TEST(test_SmallCalculatorInvoked);
  return UNITY_END();
}

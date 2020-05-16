#ifndef CALCULATOR_HEADER
#define CALCULATOR_HEADER

#include <iostream>
#include <vector>

struct Input {
  double value;
};
struct Output {
  double value;
};

// bool handles(Input const& input);
// Output compute(Input const& input);
// void log(Input const& input, Output const& output);

struct Calculator {
  struct BigCalculator {
    static bool handles(Input const& input)  { return input.value > 10; }

    static Output compute(Input const& input) {
      return Output{input.value * 5};
    }

    static void log(Input const& input, Output const& output) {
      std::cout << "BigCalculator took an input of " << input.value
                << " and produced an output of " << output.value << '\n';
    }
  };

  struct SmallCalculator {
    static bool handles(Input const& input)  { return input.value <= 10; }

    static Output compute(Input const& input) {
      return Output{input.value + 2};
    }

    static void log(Input const& input, Output const& output) {
      std::cout << "SmallCalculator took an input of " << input.value
                << " and produced an output of " << output.value << '\n';
    }
  };

  bool (*handles)(Input const& input) ;
  Output (*compute)(Input const& input);
  void (*log)(Input const& input, Output const& output);

  template <typename CalculatorImplementation>
  static Calculator createFrom() {
    return Calculator{&CalculatorImplementation::handles,
                      &CalculatorImplementation::compute,
                      &CalculatorImplementation::log};
  }

  static std::vector<Calculator> getCalculators() {
    return {Calculator::createFrom<BigCalculator>(),
            Calculator::createFrom<SmallCalculator>()};
  }
};
#endif  // !CALCULATOR_HEADER

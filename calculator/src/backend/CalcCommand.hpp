#ifndef CALC_COMMAND_HPP
#define CALC_COMMAND_HPP

#include <functional>
#include <memory>
#include <vector>

#include "CalcModel.hpp"
#include "utilities/Command.hpp"

using calculator::utility::Command;
using calculator::utility::Exception;
using std::unique;
using namespace calculator::model;
using CommandPtr = calculator::utility::CommandPtr;

namespace calculator {
namespace controller {

/**
 * base class dedicated for calculator
 */
template <class M>
class CalculatorCommand : public Command {
 protected:
  CalculatorCommand(M& m) : model_(m) {}

  M& getModel() { return model_.get(); }
  const M& getModel() const { return model_.get(); }

 private:
  std::reference_wrapper<M> model_;
};

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
template <class T = double, class M = model::CalcModel<T>>
class EnterNumber : public CalculatorCommand<M> {
 public:
  EnterNumber(T num, M& m) : CalculatorCommand<M>(m), num_(num) {}
  ~EnterNumber() = default;
  EnterNumber(const EnterNumber&) = default;

 private:
  EnterNumber(EnterNumber&&) = delete;
  EnterNumber& operator=(const EnterNumber&) = delete;
  EnterNumber& operator=(EnterNumber&&) = delete;

  using CalculatorCommand<M>::getModel;

  void executeImpl() override { getModel().push(num_); }
  void undoImpl() override { getModel().pop(); }
  auto cloneImpl() const -> CommandPtr override {
    return std::make_unique<EnterNumber>(*this);
  }
  const char* helpMessageImpl() const noexcept override {
    return "Adds a number to the stack";
  }

 private:
  T num_;
};

// swaps two numbers on the stack
// precondition: must have at least two elements on the stack
template <class T = double, class M = model::CalcModel<T>>
class SwapTopOfStack : public CalculatorCommand<M> {
 public:
  SwapTopOfStack(M& m) : CalculatorCommand<M>(m) {}
  ~SwapTopOfStack() = default;
  SwapTopOfStack(const SwapTopOfStack&) = default;

 private:
  SwapTopOfStack(SwapTopOfStack&&) = delete;
  SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
  SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;

  using CalculatorCommand<M>::getModel;

  // throws an exception if the stack size is less than two
  void checkPreconditionsImpl() const override {
    if (getModel().size() < 2) throw Exception{"Stack must have 2 elements"};
  }

  // swaps the top two stack positions
  void executeImpl() noexcept override { getModel().swapTop2(); }

  // reverses the swap operation
  void undoImpl() noexcept override { getModel().swapTop2(); }

  CommandPtr cloneImpl() const override {
    return std::make_unique<SwapTopOfStack>(*this);
  }

  const char* helpMessageImpl() const noexcept override {
    return "Swap the top two elements of the stack";
  }
};

// drop the top of the stack
// precondition: must have at least one element on the stack
template <class T = double, class M = model::CalcModel<T>>
class DropTopOfStack : public CalculatorCommand<M> {
 public:
  DropTopOfStack(M& m) : CalculatorCommand<M>(m) {}
  ~DropTopOfStack() = default;
  DropTopOfStack(const DropTopOfStack&) = default;

 private:
  DropTopOfStack(DropTopOfStack&&) = delete;
  DropTopOfStack& operator=(const DropTopOfStack&) = delete;
  DropTopOfStack& operator=(DropTopOfStack&&) = delete;

  using CalculatorCommand<M>::getModel;

  // throws an exception if the stack size is less than one
  void checkPreconditionsImpl() const override {
    if (getModel().size() < 1) throw Exception{"Stack must have one element"};
  }

  // swaps the top two stack positions
  void executeImpl() noexcept override {
    droppedNumber_ = getModel().top();
    getModel().pop();
  }

  // reverses the swap operation
  void undoImpl() noexcept override { getModel().push(droppedNumber_); }

  CommandPtr cloneImpl() const override {
    return std::make_unique<DropTopOfStack>(*this);
  }

  const char* helpMessageImpl() const noexcept override {
    return "Drop the top element from the stack";
  }
  double droppedNumber_;
};

// clear the stack
// precondition: must have at least one element on the stack
template <class T = double, class M = model::CalcModel<T>>
class ClearStack : public CalculatorCommand<M> {
 public:
  ClearStack(M& m) : CalculatorCommand<M>(m) {}
  ClearStack(const ClearStack&) = default;
  ~ClearStack() = default;

 private:
  ClearStack(ClearStack&&) = delete;
  ClearStack& operator=(const ClearStack&) = delete;
  ClearStack& operator=(ClearStack&&) = delete;
  using CalculatorCommand<M>::getModel;
  // clears the stack
  void executeImpl() noexcept override {
    stack_ = std::move(getModel().copyElements());
  }

  // restores the stack
  void undoImpl() noexcept override {
    for (auto& e : stack_) getModel().push(std::move(e), true);
  }

  CommandPtr cloneImpl() const override {
    return std::make_unique<ClearStack>(*this);
  }

  const char* helpMessageImpl() const noexcept override {
    return "Clear the stack";
  }

  std::vector<T> stack_;
};

// This shows an entirely different design using function and lambdas.
template <class T = double, class M = model::CalcModel<T>>
class BinaryCommand final : public CalculatorCommand<M> {
  using BinaryCommandOp = T(T, T);

 public:
  BinaryCommand(const std::string& help, std::function<BinaryCommandOp> f, M& m)
      : CalculatorCommand<M>(m), helpMsg_{help}, command_{std::move(f)} {}
  ~BinaryCommand() = default;
  BinaryCommand(const BinaryCommand&) = default;

 private:
  BinaryCommand(BinaryCommand&&) = delete;
  BinaryCommand& operator=(const BinaryCommand&) = delete;
  BinaryCommand& operator=(BinaryCommand&&) = delete;

  using CalculatorCommand<M>::getModel;
  // throws an exception if the stack size is less than two
  void checkPreconditionsImpl() const override {
    if (getModel().size() < 2) throw Exception{"Stack must have 2 elements"};
  }

  const char* helpMessageImpl() const noexcept override {
    return helpMsg_.c_str();
  }

  // takes two elements from the stack, applies the binary operation
  // and returns the result to the stack
  void executeImpl() noexcept override {
    top_ = getModel().top();
    getModel().pop(true);
    next_ = getModel().top();
    getModel().pop(true);
    getModel().push(command_(next_, top_));
  }

  // drops the result and returns the original two numbers to the stack
  void undoImpl() noexcept override {
    getModel().pop(true);
    getModel().push(next_, true);
    getModel().push(top_);
  }

  CommandPtr cloneImpl() const override {
    return std::make_unique<BinaryCommand<T>>(*this);
  }

  T top_;
  T next_;
  std::string helpMsg_;
  std::function<BinaryCommandOp> command_;
};

template <class T = double, class M = model::CalcModel<T>>
class UnaryCommand final : public CalculatorCommand<M> {
  using UnaryCommandOp = T(T);

 public:
  UnaryCommand(const std::string& help, std::function<UnaryCommandOp> f, M& m)
      : CalculatorCommand<M>(m), helpMsg_{help}, command_{std::move(f)} {}
  ~UnaryCommand() = default;
  UnaryCommand(const UnaryCommand&) = default;

 private:
  UnaryCommand(UnaryCommand&&) = delete;
  UnaryCommand& operator=(const UnaryCommand&) = delete;
  UnaryCommand& operator=(UnaryCommand&&) = delete;

  using CalculatorCommand<M>::getModel;
  // throws an exception if the stack size is less than two
  void checkPreconditionsImpl() const override {
    if (getModel().size() < 1) throw Exception{"Stack must have one element"};
  }

  const char* helpMessageImpl() const noexcept override {
    return helpMsg_.c_str();
  }

  // takes top element from the stack, applies the unary operation
  // and returns the result to the stack
  void executeImpl() noexcept override {
    top_ = getModel().top();
    getModel().pop(true);
    getModel().push(command_(top_));
  }

  // drops the result and returns the original one number to the stack
  void undoImpl() noexcept override {
    getModel().pop(true);
    getModel().push(top_);
  }

  CommandPtr cloneImpl() const override {
    return std::make_unique<UnaryCommand<T>>(*this);
  }

  T top_;
  std::string helpMsg_;
  std::function<UnaryCommandOp> command_;
};

template <typename T>
T identity_element(std::multiplies<T>)
{
    return T{1};
}

template <typename T>
T identity_element(std::plus<T>)
{
    return T{0};
}

template <typename T=int, typename N=size_t, typename O=std::multiplies<T>>
T power(T x, N n, O op) {
  if (n == 0) return identity_element(op);
  while ((n & 1) == 0) {
    n >>= 1;
    x = op(x, x);
  }
  T result = x;
  n >>= 1;
  while (n != 0) {
    x = op(x, x);
    if ((n & 1) != 0) result = op(result, x);
    n >>= 1;
  }
  return result;
}

template< typename T, typename N >
struct pow
{
    constexpr T operator()(T lhs, N rhs) const
    {
        return power<T, N>(std::move(lhs), std::move(rhs), std::multiplies<T>{});
    }
};


}  // namespace controller
}  // namespace calculator
#endif  // COMMAND_HPP

#ifndef CALC_COMMAND_HPP
#define CALC_COMMAND_HPP

#include<functional>
#include <memory>
#include "CalcModel.hpp"
#include "utilities/Command.hpp"

using std::unique;
using calculator::utility::Exception;
using calculator::utility::Command;
using namespace calculator::model;
using CommandPtr = calculator::utility::CommandPtr;

namespace calculator {
namespace controller {

/**
  * base class dedicated for calculator
  */
template<class M>
class CalculatorCommand : public Command
{
protected:
    CalculatorCommand(M& m):model_(m){}

    M& getModel() { return model_.get(); }
    const M& getModel() const { return model_.get(); }

private:
    std::reference_wrapper<M> model_;
};

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
template<class T = double, class M = model::CalcModel<T>>
class EnterNumber : public CalculatorCommand<M>
{
public:
    EnterNumber(T num, M& m):CalculatorCommand<M>(m),num_(num){}
    ~EnterNumber()=default;
    EnterNumber(const EnterNumber&)=default;
private:
    EnterNumber(EnterNumber&&) = delete;
    EnterNumber& operator=(const EnterNumber&) = delete;
    EnterNumber& operator=(EnterNumber&&) = delete;

    using CalculatorCommand<M>::getModel;

    void executeImpl() override
    {
        getModel().push(num_);
    }
    void undoImpl() override
    {
        getModel().pop();
    }
    auto cloneImpl() const -> CommandPtr override
    {
        return std::make_unique<EnterNumber>(*this);
    }
    const char* helpMessageImpl() const noexcept override
    {
        return "Adds a number to the stack";
    }
private:
    T num_;
};

// swaps two numbers on the stack
// precondition: must have at least two elements on the stack
template<class T = double, class M = model::CalcModel<T>>
class SwapTopOfStack : public CalculatorCommand<M>
{
public:
    SwapTopOfStack(M& m):CalculatorCommand<M>(m) {}
    ~SwapTopOfStack()=default;
    SwapTopOfStack(const SwapTopOfStack&)=default;
private:
    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;


    using CalculatorCommand<M>::getModel;

    // throws an exception if the stack size is less than two
    void checkPreconditionsImpl() const override
    {
        if( getModel().size() < 2 )
            throw Exception{"Stack must have 2 elements"};
    }

    // swaps the top two stack positions
    void executeImpl() noexcept override
    {
        getModel().swapTop2();
    }

    // reverses the swap operation
    void undoImpl() noexcept override
    {
        getModel().swapTop2();
    }

    std::unique_ptr<Command> cloneImpl() const override
    {
        return std::make_unique<SwapTopOfStack>(*this);
    }

    const char* helpMessageImpl() const noexcept override
    {
        return "Swap the top two elements of the stack";
    }
};

// This shows an entirely different design using function and lambdas.
template<class T = double, class M = model::CalcModel<T>>
class BinaryCommand final : public CalculatorCommand<M>
{
    using BinaryCommandOp = T(T, T);
public:
    BinaryCommand(const std::string& help, std::function<BinaryCommandOp> f, M& m):
        CalculatorCommand<M>(m), helpMsg_{help} , command_{std::move(f)} {}
    ~BinaryCommand() = default;
    BinaryCommand(const BinaryCommand&)=default;
private:
    BinaryCommand(BinaryCommand&&) = delete;
    BinaryCommand& operator=(const BinaryCommand&) = delete;
    BinaryCommand& operator=(BinaryCommand&&) = delete;

    using CalculatorCommand<M>::getModel;
    // throws an exception if the stack size is less than two
    void checkPreconditionsImpl() const override
    {
        if( getModel().size() < 2 )
            throw Exception{"Stack must have 2 elements"};
    }

    const char* helpMessageImpl() const noexcept override{
        return helpMsg_.c_str();
    }

    // takes two elements from the stack, applies the binary operation
    // and returns the result to the stack
    void executeImpl() noexcept override
    {
        top_ = getModel().top();
        getModel().pop(true);
        next_ = getModel().top();
        getModel().pop(true);
        getModel().push( command_(next_, top_) );
    }

    // drops the result and returns the original two numbers to the stack
    void undoImpl() noexcept override
    {
        getModel().pop(true);
        getModel().push(next_, true);
        getModel().push(top_);
    }

    CommandPtr cloneImpl() const override
    {
        return std::make_unique<BinaryCommand<T>>(*this);
    }

    T top_;
    T next_;
    std::string helpMsg_;
    std::function<BinaryCommandOp> command_;
};

}
}
#endif // COMMAND_HPP

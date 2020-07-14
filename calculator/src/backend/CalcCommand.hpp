#ifndef CALC_COMMAND_HPP
#define CALC_COMMAND_HPP

#include <memory>
#include "Stack.hpp"
#include "utilities/Command.hpp"

using std::unique;
using calculator::utility::Exception;
using calculator::utility::Command;

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
template<class T = double, class M = model::Stack<T>>
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
    std::unique_ptr<Command> cloneImpl() const
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
template<class T = double, class M = model::Stack<T>>
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

}
}
#endif // COMMAND_HPP

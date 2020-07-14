#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>
#include "Stack.hpp"

using std::unique;
using calculator::utility::Exception;

namespace calculator {
namespace controller {

// generic base class for command patter
class Command{
public:
    virtual ~Command() {}

    // executes the command
    void execute(){
        checkPreconditionsImpl();
        executeImpl();
    }

    // undoes the command
    void undo(){
        undoImpl();
    }

    // create a polymorphic copy of the command
    Command* clone() const {
        return cloneImpl();
    }

    // supplies a short help message for the command
    const char* helpMessage() const {
        return helpMessageImpl();
    }

protected:
    Command() = default;
    Command(const Command&) = default;

private:
    // this function is not pure virtual because a command that needs no preconditions
    // shouldn't be forced to check for any...thus, this defaults to not throwing
    virtual void checkPreconditionsImpl() const {}

    virtual void executeImpl() = 0;
    virtual void undoImpl() = 0;
    virtual Command* cloneImpl() const = 0;
    // all commands should have a short help
    virtual const char* helpMessageImpl() const noexcept = 0;

    Command(Command&&) = delete;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&&) = delete;
};

/**
  * base class dedicated for calculator
  */
template<class M>
class CalculatorCommand : public Command
{
protected:
    M& model_;
    CalculatorCommand(M& m):model_(m){}
};

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
template<class T = double, class M = model::Stack<T>>
class EnterNumber : public CalculatorCommand<M>
{
public:
    EnterNumber(T num, M& m):CalculatorCommand<M>(m),num_(num){}
    ~EnterNumber()=default;
private:
    EnterNumber(EnterNumber&&) = delete;
    EnterNumber& operator=(const EnterNumber&) = delete;
    EnterNumber& operator=(EnterNumber&&) = delete;
    EnterNumber(const EnterNumber&)=default;

    using CalculatorCommand<M>::model_;

    void executeImpl() override
    {
        model_.push(num_);
    }
    void undoImpl() override
    {
        model_.pop();
    }
    Command* cloneImpl() const
    {
        return new EnterNumber(*this);
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

private:
    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;
    SwapTopOfStack(const SwapTopOfStack&)=default;

    using CalculatorCommand<M>::model_;

    // throws an exception if the stack size is less than two
    void checkPreconditionsImpl() const override
    {
        if( model_.size() < 2 )
            throw Exception{"Stack must have 2 elements"};
    }

    // swaps the top two stack positions
    void executeImpl() noexcept override
    {
        model_.swapTop2();
    }

    // reverses the swap operation
    void undoImpl() noexcept override
    {
        model_.swapTop2();
    }

    Command* cloneImpl() const override
    {
        return new SwapTopOfStack{*this};
    }

    const char* helpMessageImpl() const noexcept override
    {
        return "Swap the top two elements of the stack";
    }
};

}
}
#endif // COMMAND_HPP

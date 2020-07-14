#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>
#include "Stack.hpp"

using std::unique;

namespace calculator {
namespace controller {

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

template<class T>
class CalculatorCommand : public Command
{
protected:
    model::Stack<T>& model_;
    CalculatorCommand(model::Stack<T>& m):model_(m){}
};

// accepts a number from input and adds it to the stack
// no preconditions are necessary for this command
template<class T>
class EnterNumber : public CalculatorCommand<T>
{
public:
    EnterNumber(T num, model::Stack<T>& m):CalculatorCommand<T>(m),num_(num){}
private:
    using CalculatorCommand<T>::model_;
    void executeImpl() override {model_.push(num_);}
    void undoImpl() override {model_.pop();}
    Command* cloneImpl() const { return new EnterNumber(*this); }
    const char* helpMessageImpl() const noexcept override{return "Adds a number to the stack";}
private:
    T num_;
};

// swaps two numbers on the stack
// precondition: must have at least two elements on the stack
template<class T>
class SwapTopOfStack : public Command
{
public:
    SwapTopOfStack() { }
    SwapTopOfStack(const SwapTopOfStack&);
    ~SwapTopOfStack();

private:
    SwapTopOfStack(SwapTopOfStack&&) = delete;
    SwapTopOfStack& operator=(const SwapTopOfStack&) = delete;
    SwapTopOfStack& operator=(SwapTopOfStack&&) = delete;

    // throws an exception if the stack size is less than two
    void checkPreconditionsImpl() const override;

    // swaps the top two stack positions
    void executeImpl() noexcept override;

    // reverses the swap operation
    void undoImpl() noexcept override;

    SwapTopOfStack* cloneImpl() const override;

    const char* helpMessageImpl() const noexcept override;
};

}
}
#endif // COMMAND_HPP

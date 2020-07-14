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

    void execute(){
        checkPreconditionsImpl();
        executeImpl();
    }

    void undo(){
        undoImpl();
    }

    Command* clone() const {
        return cloneImpl();
    }

protected:
    Command() = default;
    Command(const Command&) = default;

private:
    virtual void checkPreconditionsImpl() const {}
    virtual void executeImpl() = 0;
    virtual void undoImpl() = 0;
    virtual Command* cloneImpl() const = 0;

    Command(Command&&) = delete;
    Command& operator=(const Command&) = delete;
    Command& operator=(Command&&) = delete;
};

template<class T>
class EnterNumber : public Command
{
public:
    EnterNumber(T num, model::Stack<T>& m):num_(num),model_(m){}
private:
    EnterNumber(const EnterNumber& rhs) :
        Command{rhs},
        num_(rhs.num_),
        model_(rhs.model_) {}

    void executeImpl() override {model_.push(num_);}
    void undoImpl() override {model_.pop();}
    Command* cloneImpl() const { return new EnterNumber(*this); }
private:
    T num_;
    model::Stack<T>& model_;
};

}
}
#endif // COMMAND_HPP

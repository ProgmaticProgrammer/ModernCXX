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
        executeImp();
    }

    void undo(){
        undoImp();
    }

    Command* clone() const {
        return cloneImp();
    }

protected:
    Command() = default;
    Command(const Command&) = default;

private:
    virtual void checkPreconditionsImpl() const {}
    virtual void executeImp() = 0;
    virtual void undoImp() = 0;
    virtual Command* cloneImp() const = 0;

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

    void executeImp() override {model_.push(num_);}
    void undoImp() override {model_.pop();}
    Command* cloneImp() const { return new EnterNumber(*this); }
private:
    T num_;
    model::Stack<T>& model_;
};

}
}
#endif // COMMAND_HPP

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Stack.hpp"

namespace calculator {
namespace controller {

class Command{
public:
    void execute(){executeImp();}
    void undo(){undoImp();}
private:
    virtual void executeImp() = 0;
    virtual void undoImp() = 0;
};

template<class T>
class EnterNumber : public Command
{
public:
    EnterNumber(T num, model::Stack<T>& m):num_(num),model_(m){}
private:
    void executeImp() override {model_.push(num_);}
    void undoImp() override {model_.pop();}
private:
    T num_;
    model::Stack<T>& model_;
};

}
}
#endif // COMMAND_HPP

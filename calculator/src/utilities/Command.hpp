#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>

namespace calculator {
namespace utility {

// generic base class for command patter
class Command {
 public:
  virtual ~Command() {}

  // executes the command
  void execute() {
    checkPreconditionsImpl();
    executeImpl();
  }

  // undoes the command
  void undo() { undoImpl(); }

  // create a polymorphic copy of the command
  std::unique_ptr<Command> clone() const { return cloneImpl(); }

  // supplies a short help message for the command
  const char* helpMessage() const { return helpMessageImpl(); }

 protected:
  Command() = default;
  Command(const Command&) = default;

 private:
  // this function is not pure virtual because a command that needs no
  // preconditions shouldn't be forced to check for any...thus, this defaults to
  // not throwing
  virtual void checkPreconditionsImpl() const {}

  virtual void executeImpl() = 0;
  virtual void undoImpl() = 0;
  virtual std::unique_ptr<Command> cloneImpl() const = 0;
  // all commands should have a short help
  virtual const char* helpMessageImpl() const noexcept = 0;

  Command(Command&&) = delete;
  Command& operator=(const Command&) = delete;
  Command& operator=(Command&&) = delete;
};

}  // namespace utility
}  // namespace calculator
#endif  // COMMAND_HPP

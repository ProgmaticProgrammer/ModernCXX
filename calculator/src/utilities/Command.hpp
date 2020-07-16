#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstddef>
#include <memory>

namespace calculator {
namespace utility {

class Command;
using CommandPtr = std::unique_ptr<Command/*, decltype(&CommandDeleter)*/>;

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
  auto clone() const -> CommandPtr { return cloneImpl(); }

  // supplies a short help message for the command
  const char* helpMessage() const { return helpMessageImpl(); }

  // Deletes commands. This should only be overridden in plugins. By default,
  // simply deletes command. In plugins, delete must happen in the plugin.
  virtual void deallocate() {delete this;}

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
  virtual CommandPtr cloneImpl() const = 0;
  // all commands should have a short help
  virtual const char* helpMessageImpl() const noexcept = 0;

  Command(Command&&) = delete;
  Command& operator=(const Command&) = delete;
  Command& operator=(Command&&) = delete;
};

inline void CommandDeleter(Command* p)
{
    p->deallocate();
}

inline void CommandDeleter(decltype(nullptr))
{
}

template<typename T, typename... Args>
auto MakeCommandPtr(Args&&... args) -> CommandPtr
{
    return CommandPtr(new T(std::forward<Args>(args)...)/*, &CommandDeleter*/);
}

inline auto MakeCommandPtr(Command* p) -> CommandPtr
{
    return CommandPtr(p/*, &CommandDeleter*/);
}

}  // namespace utility
}  // namespace calculator
#endif  // COMMAND_HPP

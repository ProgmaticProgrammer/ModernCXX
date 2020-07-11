#ifndef STACK_HPP
#define STACK_HPP

#include <algorithm>
#include <deque>
#include <exception>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

#include "Exception.hpp"
#include "Publisher.hpp"

namespace calculator {
namespace model {

using utility::EventData;
using utility::Exception;
using utility::Publisher;

// make sure each condition and message match in same order
enum ErrorConditions { Empty = 0, TooFewArguments, Unknown };
const static char* ErrorMessages[] = {
    "Attempting to pop empty stack",
    "Need at least two stack elements to swap top", "Unknown error"};

/**
 * Stack Error Event, wrap error condition
 */
class StackEventData : public EventData,
                       public std::enable_shared_from_this<StackEventData> {
 public:
  explicit StackEventData(ErrorConditions e) : err_(e) {}

  const char* message() const { return ErrorMessages[err_]; }

  ErrorConditions error() const { return err_; }

  auto getSharedEventData() { return shared_from_this(); }

 private:
  ErrorConditions err_;
};

const static shared_ptr<StackEventData> EmptyError =
    std::make_shared<StackEventData>(ErrorConditions::Empty);
const static shared_ptr<StackEventData> TooFewArgumentsError =
    std::make_shared<StackEventData>(ErrorConditions::TooFewArguments);
/**
 * Stack with Publish code resue
 */
template <class T>
class Stack : private Publisher {
 public:
  using value_type = T;
  // Two events for publish
  static const std::string StackChanged;
  static const std::string StackError;

  Stack() {
    registerEvent(StackChanged);
    registerEvent(StackError);
  }
  ~Stack() = default;

  void push(T d) {
    stack_.push_back(std::move(d));
    Publisher::notify(Stack::StackChanged, nullptr);
  }
  void pop() {
    if (!stack_.empty()) {
      stack_.pop_back();
      Publisher::notify(Stack::StackChanged, nullptr);
    } else {
      Publisher::notify(Stack::StackError, EmptyError->getSharedEventData());
      throw Exception{EmptyError->message()};
    }
  }
  T& top() { return const_cast<T&>(static_cast<const Stack&>(*this).top()); }
  const T& top() const {
    if (stack_.empty()) {
      Publisher::notify(Stack::StackError, EmptyError->getSharedEventData());
      throw Exception{EmptyError->message()};
    }
    return stack_.back();
  }

  void swapTop2() {
    if (stack_.size() >= 2) {
      auto first = std::prev(stack_.end(), 1);
      auto second = std::prev(stack_.end(), 2);
      std::iter_swap(first, second);
    } else {
      Publisher::notify(Stack::StackError,
                        TooFewArgumentsError->getSharedEventData());
      throw Exception{TooFewArgumentsError->message()};
    }
  }

  std::vector<T> copyElements(
      size_t n = std::numeric_limits<size_t>::max()) const {
    std::vector<T> result;
    n = std::min(n, stack_.size());
    std::copy_n(stack_.rbegin(), n, std::back_inserter(result));
    return std::move(result);
  }

  size_t size() const { return stack_.size(); }
  void clear() { stack_.clear(); }

 private:
  std::deque<T> stack_;

  Stack(const Stack&) = delete;
  Stack(Stack&&) = delete;
  Stack& operator=(const Stack&) = delete;
  Stack& operator=(Stack&&) = delete;
};

}  // namespace model
}  // namespace calculator

#endif  // STACK_HPP

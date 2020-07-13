#ifndef STACK_HPP
#define STACK_HPP

#include <algorithm>
#include <deque>
#include <exception>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

#include "../utilities/Publisher.hpp"
#include "../utilities/Exception.hpp"

namespace calculator {
namespace model {

using utility::EventData;
using utility::Exception;
using utility::Publisher;

/**
 * Stack Error Event, wrap error condition
 */
class StackEvent : public EventData,
                       public std::enable_shared_from_this<StackEvent> {
 public:
  enum ErrorConditions { Empty = 0, TooFewArguments, Unknown };
  explicit StackEvent(ErrorConditions e) : err_(e) {}

  const char* message() const { return ErrorMessages[err_]; }

  ErrorConditions error() const { return err_; }

  auto getSharedEventData() { return shared_from_this(); }

  // make sure each condition and message match in same order

  static const char* ErrorMessages[];
  static const shared_ptr<StackEvent> EmptyError;
  static const shared_ptr<StackEvent> TooFewArgumentsError;

 private:
  ErrorConditions err_;
};

/**
 * Stack, the implements-a relationship with Publisher
 */
template <class T>
class Stack : private Publisher {
 public:
  using value_type = T;
  using Publisher::attach;
  using Publisher::detach;

  // Two subjects for observers who're interested in
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
      Publisher::notify(Stack::StackError, StackEvent::EmptyError->getSharedEventData());
      throw Exception{StackEvent::EmptyError->message()};
    }
  }
  T& top() { return const_cast<T&>(static_cast<const Stack&>(*this).top()); }
  const T& top() const {
    if (stack_.empty()) {
      Publisher::notify(Stack::StackError, StackEvent::EmptyError->getSharedEventData());
      throw Exception{StackEvent::EmptyError->message()};
    }
    return stack_.back();
  }

  void swapTop2() {
    if (stack_.size() >= 2) {
      auto first = std::prev(stack_.end(), 1);
      auto second = std::prev(stack_.end(), 2);
      std::iter_swap(first, second);
      Publisher::notify(Stack::StackChanged, nullptr);
    } else {
      Publisher::notify(Stack::StackError,
                        StackEvent::TooFewArgumentsError->getSharedEventData());
      throw Exception{StackEvent::TooFewArgumentsError->message()};
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
  void clear() { stack_.clear(); Publisher::notify(Stack::StackChanged, nullptr);}

 private:
  std::deque<T> stack_;

  Stack(const Stack&) = delete;
  Stack(Stack&&) = delete;
  Stack& operator=(const Stack&) = delete;
  Stack& operator=(Stack&&) = delete;
};

template <class T>
const string Stack<T>::StackChanged = "stackChanged";

template <class T>
const string Stack<T>::StackError = "error";

}  // namespace model
}  // namespace calculator

#endif  // STACK_HPP

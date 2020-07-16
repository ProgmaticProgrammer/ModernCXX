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
template <class T=double>
class CalcModel : private Publisher {
 public:
  using value_type = T;
  using Publisher::attach;
  using Publisher::detach;

  // Two subjects for observers who're interested in
  static const std::string ModelChanged;
  static const std::string ModelError;

  CalcModel() {
    registerEvent(ModelChanged);
    registerEvent(ModelError);
  }
  ~CalcModel(){}

  void push(T d, bool suppressChangeEvent = false) {
    stack_.push_back(std::move(d));
    if(!suppressChangeEvent) Publisher::notify(CalcModel::ModelChanged, nullptr);
  }
  void pop(bool suppressChangeEvent = false) {
    if (!stack_.empty()) {
      stack_.pop_back();
      if(!suppressChangeEvent) Publisher::notify(CalcModel::ModelChanged, nullptr);
    } else {
      Publisher::notify(CalcModel::ModelError, StackEvent::EmptyError->getSharedEventData());
      throw Exception{StackEvent::EmptyError->message()};
    }
  }
  T& top() { return const_cast<T&>(static_cast<const CalcModel&>(*this).top()); }
  const T& top() const {
    if (stack_.empty()) {
      Publisher::notify(CalcModel::ModelError, StackEvent::EmptyError->getSharedEventData());
      throw Exception{StackEvent::EmptyError->message()};
    }
    return stack_.back();
  }

  void swapTop2() {
    if (stack_.size() >= 2) {
      auto first = std::prev(stack_.end(), 1);
      auto second = std::prev(stack_.end(), 2);
      std::iter_swap(first, second);
      Publisher::notify(CalcModel::ModelChanged, nullptr);
    } else {
      Publisher::notify(CalcModel::ModelError,
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
  void clear() {
      stack_.clear();
      Publisher::notify(CalcModel::ModelChanged, nullptr);
  }

 private:
  std::deque<T> stack_;

  CalcModel(const CalcModel&) = delete;
  CalcModel(CalcModel&&) = delete;
  CalcModel& operator=(const CalcModel&) = delete;
  CalcModel& operator=(CalcModel&&) = delete;
};

template <class T>
const string CalcModel<T>::ModelChanged = "modelChanged";

template <class T>
const string CalcModel<T>::ModelError = "modelError";

}  // namespace model
}  // namespace calculator

#endif  // STACK_HPP

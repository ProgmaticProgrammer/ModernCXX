#ifndef STACK_HPP
#define STACK_HPP

#include <algorithm>
#include <deque>
#include <exception>
#include <limits>
#include <memory>
#include <vector>

namespace calculator {
namespace model {

template <class T>
class Stack {
 public:
  using value_type = T;

  Stack() = default;
  ~Stack() = default;

  void push(T d) { stack_.push_back(std::move(d)); }
  void pop() {
    if (!stack_.empty()) {
      stack_.pop_back();
    }
  }
  T& top() {
    if (stack_.empty()) throw std::out_of_range("stack is empty");
    return stack_.back();
  }
  const T& top() const {
    if (stack_.empty()) throw std::out_of_range("stack is empty");
    return stack_.back();
  }

  void swapTop2() {
    if (stack_.size() >= 2) {
      auto first = stack_.back();
      stack_.pop_back();
      auto second = stack_.back();
      stack_.pop_back();
      stack_.push_back(std::move(first));
      stack_.push_back(std::move(second));
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

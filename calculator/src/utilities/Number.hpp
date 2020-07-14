#ifndef NUMBER_H
#define NUMBER_H

#include <memory>

using std::make_unique;
using std::unique_ptr;

class AnyNumber {
 public:
  template <class Number = double>
  explicit AnyNumber(Number n) : p_(make_unique<model<Number>>(std::move(n))) {}

  AnyNumber(const AnyNumber& rhs) : p_(rhs.p_->clone()) {}
  AnyNumber(AnyNumber&&) noexcept = default;
  AnyNumber& operator=(AnyNumber rhs) {
    std::swap(p_, rhs.p_);
    return *this;
  }
  ~AnyNumber() = default;

  int operator-() const { return p_->negate(); }
  bool operator!() const { return p_->not_(); }

 private:
  struct concept {
    virtual unique_ptr<concept> clone() const = 0;
    virtual int negate() const = 0;
    virtual bool not_() const = 0;
    virtual ~concept() = default;
  };

  template <class Number>
  struct model : public concept {
    Number num_;

    explicit model(Number n) : num_(std::move(n)) {}

    unique_ptr<concept> clone() const override {
      return std::make_unique<model>(num_);
    }
    int negate() const override { return -num_; }
    bool not_() const override { return !num_; }
  };

  unique_ptr<concept> p_ = nullptr;
};

int __main() {
  AnyNumber one(1);
  AnyNumber two(2.0);
  assert(-one == -1);
  assert(!two == false);
}

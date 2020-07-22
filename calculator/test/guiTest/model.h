// Copyright 2020 Lei Zhang
// Contact: lei.zhang.auckland@gmail.com
//
// This file is part of Calculator.
//
// Calculator is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Calculator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtMath>
#include <cmath>

namespace CalcModel {

enum Operator {
  Plus = 0,
  Minus,
  Multiplies,
  Divides,
  Percent,
  Negate,
  Pow,
  Reciproc,
  Sqrt,
  Enter,
  UNKNOWN_OPERATOR,
};

QString to_string(Operator val);
Operator to_operator(const QString &val);

using T = double;
// template<class T=double>
class Calculator {
 public:
  virtual ~Calculator() = default;

 public:
  void input_operand(T d) { input_operand_impl(d); }
  void input_operator(Operator o) { input_operator_impl(o); }
  bool get_result(T &r) { return get_result_impl(r); }

 private:
  virtual void input_operand_impl(T) = 0;
  virtual void input_operator_impl(Operator) = 0;
  virtual bool get_result_impl(T &r) = 0;
};

// template< class T=double>
class Model : public QObject, public Calculator {  //<T>
  Q_OBJECT

 public:
  Model()
      : QObject(),
        result_(0),
        lhs_(0.0),
        rhs_(HUGE_VAL),
        op_(UNKNOWN_OPERATOR),
        state_(Empty),
        count_(0) {}

  enum State {
    Empty,
    LhsReady,
    WaitingForRhs,
    BothReady,
    Calculating,
    // READY,
    // ERROR
  };

  void reset() {
    setResult(0.0);
    setLhs(0.0);
    setRhs(HUGE_VAL);
    setOp(UNKNOWN_OPERATOR);
    setState(Empty);
    count_ = 0;
  }

  T result() const { return result_; }

 public slots:
  void setLhs(T lhs) { lhs_ = lhs; }
  void setRhs(T rhs) { rhs_ = rhs; }
  void setOp(Operator op) { op_ = op; }
 signals:
  // void resultChanged(T newResult);
  void resultChanged(QString newResult);

 public:  // for test
  State state() const { return state_; }
  T lhs() const { return lhs_; }
  T rhs() const { return rhs_; }
  Operator op() const { return op_; }

 private:
  // implementation of virtual funcs in Calculator interface
  void input_operand_impl(T d) override {
    ++count_;

    if (count_ < 2) {
      setLhs(d);
      setState(LhsReady);
    } else {
      if (state() != BothReady) {
        setRhs(d);
        setState(BothReady);
      } else {
        setLhs(d);
      }
    }
  }

  void input_operator_impl(Operator o) override {
    if (isBinaryOp(o)) {
      calculateAppending();
      setOp(o);
      setState(WaitingForRhs);
    } else if (isUnaryOp(o)) {
      if (state() == Model::State::BothReady) {
        setRhs(doUnaryCalculation(o, rhs()));
      } else if (state() == Model::State::LhsReady) {
        setLhs(doUnaryCalculation(o, lhs()));
      }
    } else if (isEnter(o)) {
      auto result = doBinaryCalculation(op(), lhs(), rhs());
      setResult(result);
      setLhs(result);
      setState(LhsReady);
    }
  }

  bool get_result_impl(T &r) {
    if (state() == BothReady && passCheck()) {
      r = doBinaryCalculation(op(), lhs(), rhs());
      return true;
    } else if (state() == LhsReady) {
      r = result();
      return true;
    } else if (state() == WaitingForRhs) {
      r = lhs();
      return true;
    } else {
      return false;
    }
  }

 private:
  void setResult(T result) {
    if (result_ != result) {
      result_ = std::move(result);
      emit resultChanged(QString::number(result_));
    }
  }
  void setState(const State &state) { state_ = state; }

 private:
  bool passCheck() const { return count_ >= 2 && op() != UNKNOWN_OPERATOR; }

  // calculation precondition check
  bool isPreconditionMet(Operator op, T operand) {
    if ((op == Sqrt && operand < 0.0) ||
        ((op == Reciproc || op == Divides) && operand == 0.0) ||
        (op == Negate && operand != 0.0)) {
      return false;
    } else {
      return true;
    }
  }

  void calculateAppending() {
    if (state() == BothReady && op() != UNKNOWN_OPERATOR) {
      setLhs(doBinaryCalculation(op(), lhs(), rhs()));
    }
  }

  // calculation of unary and binary ops
  T doUnaryCalculation(Operator o, T operand) {
    T result = HUGE_VAL;

    if (o == Sqrt) {
      result = std::sqrt(operand);
    } else if (o == Pow) {
      result = std::pow(operand, 2.0);
    } else if (o == Reciproc) {
      result = 1.0 / operand;
    }

    return result;
  }

  T doBinaryCalculation(Operator o, T lhs, T rhs) {
    T result = HUGE_VAL;

    if (isPreconditionMet(o, rhs)) {
      if (o == Plus) {
        result = lhs + rhs;
      } else if (o == Minus) {
        result = lhs - rhs;
      } else if (o == Multiplies) {
        result = lhs * rhs;
      } else if (o == Divides) {
        result = lhs / rhs;
      }
    }

    return result;
  }
  // predicts
  bool isEnter(Operator o) const { return o == Enter; }
  bool isUnaryOp(Operator op) const {
    return op == Pow || op == Reciproc || op == Sqrt || op == Negate;
  }

  bool isBinaryOp(Operator op) const {
    return op == Plus || op == Minus || op == Multiplies || op == Divides;
  }
  // internal state and data members
  T result_;
  T lhs_;
  T rhs_;
  Operator op_;
  State state_;

 private:
  size_t count_;
};

// extern Model defModel;

}  // namespace CalcModel
#endif  // GUIINTERFACE_H

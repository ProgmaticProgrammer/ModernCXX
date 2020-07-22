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

class Calculator {
 public:
  virtual ~Calculator() = default;

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
    Unknown
  };

 public:
  void input_operand(double d) { input_operand_impl(d); }
  void input_operator(Operator o) { input_operator_impl(o); }
  bool get_result(double &r) { return get_result_impl(r); }

 private:
  virtual void input_operand_impl(double) = 0;
  virtual void input_operator_impl(Operator) = 0;
  virtual bool get_result_impl(double &r) = 0;
};

QString to_string(Calculator::Operator val);
Calculator::Operator to_operator(const QString &val);

class Model : public QObject, public Calculator {
  Q_OBJECT

 public:
  Model()
      : QObject(),
        result_("0"),
        lhs_(0.0),
        rhs_(HUGE_VAL),
        op_(""),
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
    setResult("0");
    setLhs(0.0);
    setRhs(HUGE_VAL);
    setOp("");
    setState(Empty);
    count_ = 0;
  }

  QString result() const { return result_; }

 public slots:
  void setLhs(double lhs) { lhs_ = lhs; }
  void setRhs(double rhs) { rhs_ = rhs; }
  void setOp(const QString &op) { op_ = op; }
 signals:
  void resultChanged(QString newResult);

 public:  // for test
  State state() const { return state_; }
  double lhs() const { return lhs_; }
  double rhs() const { return rhs_; }
  QString op() const { return op_; }

 public:
  bool passCheck() { return count_ >= 2 && op() != ""; }

  void input_operand_impl(double d) override {
    ++count_;

    if (count_ < 2) {
      setLhs(d);
      setState(LhsReady);
    }
    else {
      setRhs(d);
      setState(BothReady);
    }
  }

  void calculateAppending() {
    if (state() == BothReady && !op().isEmpty()) {
      setLhs(doBinaryCalculation(to_operator(op()), lhs(), rhs()));
    }
  }

  void input_operator_impl(Operator o) override {
    auto op_str = to_string(o);

    if (isBinaryOp(o)) {
      calculateAppending();
      setOp(op_str);
      setState(WaitingForRhs);
    } else if (isUnaryOp(o)) {
      if (state() == Model::State::BothReady) {
        setRhs(doUnaryCalculation(o, rhs()));
      } else if (state() == Model::State::BothReady) {

      }
    } else if (isEnter(o)) {
      auto result = doBinaryCalculation(to_operator(op()), lhs(), rhs());
      setResult(QString::number(result));
      setLhs(result);
      setState(LhsReady);
    }
  }

  bool get_result_impl(double &r) {
    if (state() == BothReady && passCheck()) {
      r = doBinaryCalculation(to_operator(op()), lhs(), rhs());
      return true;
    } else if (state() == LhsReady) {
      r = result().toDouble();
      return true;
    } else if (state() == WaitingForRhs) {
      r = lhs();
      return true;
    } else {
      return false;
    }
  }

 private:
  void setResult(const QString &result) {
    if (result_ != result) {
      result_ = result;
      emit resultChanged(result_);
    }
  }
  void setState(const State &state) { state_ = state; }

 private:
  bool checkOpPrecondition(Operator op, double operand) {
    if ((op == Sqrt && operand < 0.0) ||
        ((op == Reciproc || op == Divides) && operand == 0.0) ||
        (op == Negate && operand != 0.0)) {
      return false;
    } else {
      return true;
    }
  }

  double doUnaryCalculation(Operator o, double operand) {
    double result = HUGE_VAL;

    if (o == Sqrt) {
      result = std::sqrt(operand);
    } else if (o == Pow) {
      result = std::pow(operand, 2.0);
    } else if (o == Reciproc) {
      result = 1.0 / operand;
    }

    return result;
  }

  double doBinaryCalculation(Operator o, double lhs, double rhs) {
    double result = HUGE_VAL;

    if (checkOpPrecondition(o, rhs)) {
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
  bool isEnter(Operator o) const { return o == Enter; }
  bool isUnaryOp(Operator op) const {
    return op == Pow || op == Reciproc || op == Sqrt || op == Negate;
  }

  bool isBinaryOp(Operator op) const {
    return op == Plus || op == Minus || op == Multiplies || op == Divides;
  }
  QString result_;
  double lhs_;
  double rhs_;
  QString op_;
  State state_;

 private:
  size_t count_;
};

extern Model defModel;

#endif  // GUIINTERFACE_H

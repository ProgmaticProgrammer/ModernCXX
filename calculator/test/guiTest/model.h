#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <cmath>
#include <QtMath>


class Calculator {
 public:
  virtual ~Calculator() = default;

  enum Operator {
    Plus = 0,
    Minus,
    Multiplies,
    Divides,
    Modulus,
    Negate,
    Pow,
    Reciproc,
    Sqrt,
    Enter,
  };

  friend QString to_string (Calculator::Operator val);

 public:
  void input_operand(double d) { input_operand_impl(d); }
  void input_operator(Operator o) { input_operator_impl(o); }
  bool get_result(double &r) { return get_result_impl(r); }

 private:
  virtual void input_operand_impl(double) = 0;
  virtual void input_operator_impl(Operator) = 0;
  virtual bool get_result_impl(double &r) = 0;
};

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

  QString result() const {return result_;}

 public slots:
  void setLhs(double lhs) {lhs_ = lhs; setState(LhsReady);}
  void setRhs(double rhs) {rhs_ = rhs;}
  void setOp(const QString &op) {op_ = op;}
 signals:
  void resultChanged(QString newResult);

 public:  // for test
  State state() const {return state_;}
  double lhs() const{return lhs_;}
  double rhs() const{return rhs_;}
  QString op() const{return op_;}

 public:
  bool passCheck() { return count_ >= 2 && op() != ""; }



  void input_operand_impl(double d) override {
    ++count_;

    if (count_ < 2)
      setLhs(d);
    else {
      setRhs(d);
      setState(BothReady);
    }
  }

  void calculateAppending()
  {
      if (!op().isEmpty())  calculate();
  }

  void input_operator_impl(Operator o) override {
      auto op_str = to_string(o);

      if (isBinaryOp(op_str)) {
          calculateAppending();
          setOp(op_str);
          setState(WaitingForRhs);
      } else if (isUnaryOp(op_str)) {
          if (state() == Model::State::BothReady) {
              setRhs(doUnaryCalculation(op_str, rhs()));
          }
      } else {
          calculate();
      }
  }

  double calculate() {
      auto result = lhs() + rhs();
      setResult(QString::number(result));
      setLhs(result);
      setState(LhsReady);
      return result;
  }

  bool get_result_impl(double &r) {
    if (state() == BothReady && passCheck()) {
      r = calculate();
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
  void setResult(const QString &result){
      if (result_ != result) {
          result_ = result;
          emit resultChanged(result_);
      }
  }
  void setState(const State &state){
      state_ = state;
  }


 private:
 bool checkOpPrecondition(const QString& op, double operand){
     if ((op == tr("\u221a") && operand < 0.0) ||
         (op == tr("1/x") && operand == 0.0) ||
         (op == tr("\303\267") && operand == 0.0)) {
         return false;
     } else {
         return true;
     }
 }
 double doUnaryCalculation(const QString& clickedOperator, double operand){
     double result = HUGE_VAL;

     if (clickedOperator == tr("\u221a")) {
         result = std::sqrt(operand);
     } else if (clickedOperator == tr("x\302\262")) {
         result = std::pow(operand, 2.0);
     } else if (clickedOperator == tr("1/x")) {
         result = 1.0 / operand;
     }

//     if (bUpdate) {
//         setResult(QString::number(result));

//         if (state() == LhsReady) {
//             setLhs(result);
//         } else if (state() == BothReady) {
//             setRhs(result);
//         }
//     }

     return result;
 }
 double doBinaryCalculation(double operand){
     double result = HUGE_VAL;
     QString o = op();
     setRhs(operand);

     if (o != "" && checkOpPrecondition(o, operand)) {
         if (o == tr("+")) {
             result = lhs() + rhs();
         } else if (o == tr("-")) {
             result = lhs() - rhs();
         } else if (o == tr("\303\227")) {
             result = lhs() * rhs();
         } else if (o == tr("\303\267")) {
             result = lhs() / rhs();
         }
         setLhs(result);
         setResult(QString::number(result));
     }

     return result;
 }
 bool isUnaryOp(const QString& op) const {
     return op == tr("\u221a") || op == tr("1/x") || op == tr("x\302\262");
 }
 bool isBinaryOp(const QString& op) const {
     return op == tr("\303\267") || op == "\303\227" || op == tr("-") ||
            op == tr("+");
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

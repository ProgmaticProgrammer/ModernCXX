#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <cmath>

//#include <QWidget>
//#include <QToolButton>
//
// QT_BEGIN_NAMESPACE
// class QLineEdit;
// QT_END_NAMESPACE

// class Button : public QToolButton
//{
//    Q_OBJECT

// public:
//    explicit Button(const QString &text, QWidget *parent = nullptr);

//    QSize sizeHint() const override;
//};

class Calculator {
 public:
  virtual ~Calculator() = default;

  enum Operator {
    Plus,
    Minus,
    Multiplies,
    Divides,
    Modulus,
    Negate,
    Pow,
    Rec,
    Sqrt,
    Reciproc
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

class Model : public QObject, public Calculator {
  Q_OBJECT

 public:
  Model()
      : QObject(),
        result_("0"),
        lhs_(0.0),
        rhs_(HUGE_VAL),
        op_(""),
        state_(WAITING_LHS),
        count_(0) {}

  enum State {
    WAITING_LHS,
    LHS_INPUTTING,
    WAITING_RHS,
    RHS_INPUTTING,
    // READY,
    // ERROR
  };

  void reset() {
    setResult("0");
    setLhs(0.0);
    setRhs(HUGE_VAL);
    setOp("");
    setState(WAITING_LHS);
    count_ = 0;
  }

  QString result() const;

 public slots:
  void setLhs(double lhs);
  void setRhs(double rhs);
  void setOp(const QString &op);
 signals:
  void resultChanged(QString newResult);

 public:  // for test
  State state() const;
  double lhs() const;
  double rhs() const;
  QString op() const;

 public:
  bool passCheck() { return count_ >= 2 && op() != ""; }

  double calculate() {
    auto result = lhs() + rhs();
    setResult(QString::number(result));
    return result;
  }

  void input_operand_impl(double d) override {
    ++count_;

    if (count_ < 2)
      setLhs(d);
    else
      setRhs(d);
  }
  void input_operator_impl(Operator) override { setOp("+"); }
  bool get_result_impl(double &r) {
    if (passCheck()) {
      r = calculate();
      return true;
    } else
      return false;
  }

 private:
  void setResult(const QString &result);
  void setState(const State &state);

 private:
  QString result_;
  double lhs_;
  double rhs_;
  QString op_;
  State state_;

 private:
  size_t count_;
};

extern Model defModel;

// class GuiInterface : public QWidget
//{
//    Q_OBJECT

// public:
//    explicit GuiInterface(QWidget *parent = nullptr, Model *model =
//    &defModel);

// private slots:
//    void digitClicked();
//    void operatorClicked();
//    void pointClicked();
//    void equalClicked();

//    void changeSignClicked();

//    void backspaceClicked();
//    void clear();
//    void clearAll();

//    void clearMemory();
//    void readMemory();
//    void setMemory();
//    void addToMemory();

// private:

//    bool checkOpPrecondition(const QString& op, double operand);
//    double doUnaryCalculation(const QString& clickedOperator, double operand);
//    double doBinaryCalculation(double);
//    bool isUnaryOp(const QString& op) const;
//    bool isBinaryOp(const QString& op) const;

//    Button *createButton(const QString &text, const char *member);
//    friend class GuiInterfaceTest;
//    QLineEdit *expression;
//    QLineEdit *recentInput;

//    enum { NumDigitButtons = 10 };
//    Button *digitButtons[NumDigitButtons];
// private:
//    Model *model_;
//};

#endif  // GUIINTERFACE_H

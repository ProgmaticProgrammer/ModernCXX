#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QToolButton>
#include <QWidget>
#include <cmath>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class Button : public QToolButton {
  Q_OBJECT

 public:
  explicit Button(const QString &text, QWidget *parent = nullptr);

  QSize sizeHint() const override;
};

class Model : public QObject {
  Q_OBJECT
 public:
  Model()
      : QObject(),
        result_("0"),
        lhs_(0.0),
        rhs_(HUGE_VAL),
        op_(""),
        state_(WAITING_LHS) {}

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
  }

  QString result() const;
  void setResult(const QString &result);

  State state() const;
  void setState(const State &state);

  double lhs() const;
  void setLhs(double lhs);

  double rhs() const;
  void setRhs(double rhs);

  QString op() const;
  void setOp(const QString &op);

 public:
     bool checkOpPrecondition(const QString &op, double operand);
     double doUnaryCalculation(const QString &clickedOperator, double operand);
     double doBinaryCalculation(double);
     bool isUnaryOp(const QString &op) const;
     bool isBinaryOp(const QString &op) const;
 signals:
  void resultChanged(QString newResult);

 private:


  QString result_;
  double lhs_;
  double rhs_;
  QString op_;
  State state_;
};

extern Model defModel;

class GuiInterface : public QWidget {
  Q_OBJECT
 public:
  explicit GuiInterface(QWidget *parent = nullptr, Model *model = &defModel);

 private slots:
  void digitClicked();
  void operatorClicked();
  void pointClicked();
  void equalClicked();

  void changeSignClicked();

  void backspaceClicked();
  void clear();
  void clearAll();

  void clearMemory();
  void readMemory();
  void setMemory();
  void addToMemory();

 private:
  Model *model_;
  Button *createButton(const QString &text, const char *member);

  QLineEdit *expression;
  QLineEdit *recentInput;

  enum { NumDigitButtons = 10 };
  Button *digitButtons[NumDigitButtons];
};

#endif  // GUIINTERFACE_H

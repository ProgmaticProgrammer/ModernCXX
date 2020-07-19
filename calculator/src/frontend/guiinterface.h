#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QtWidgets/QWidget>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class Button : public QToolButton
{
    Q_OBJECT

public:
    explicit Button(const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
};

class GuiInterface : public QWidget
{
    Q_OBJECT
public:
    explicit GuiInterface(QWidget *parent = nullptr);

    enum State {
        WAITING_LHS = 0,
        LHS_INPUTTING,
        WAITING_RHS,
        RHS_INPUTTING,
        READY
    };

    QString result() const;
    void setResult(const QString &result);

    State state() const;
    void setState(const State& state);

    double lhs() const;
    void setLhs(double lhs);

    double rhs() const;
    void setRhs(double rhs);

    QString op() const;
    void setOp(const QString &op);

signals:
    void resultChanged(QString newResult);

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

    bool checkOpPrecondition(const QString& op, double operand);
    double UnaryOpCalculation(const QString& clickedOperator, double operand);
    double doBinaryCalculation(const QString& op);
    bool isUnaryOp(const QString& op) const;
    bool isBinaryOp(const QString& op) const;

    Button *createButton(const QString &text, const char *member);

    QLineEdit *expression;
    QLineEdit *recentInput;

    QString result_;
    double lhs_;
    double rhs_;
    QString op_;
    State state_;

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
};

#endif // GUIINTERFACE_H

#include "guiinterface.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QtMath>

Button::Button(const QString &text, QWidget *parent) : QToolButton(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setText(text);
}

QSize Button::sizeHint() const {
  QSize size = QToolButton::sizeHint();
  size.rheight() += 20;
  size.rwidth() = qMax(size.width(), size.height());
  return size;
}

Button *GuiInterface::createButton(const QString &text, const char *member) {
  Button *button = new Button(text);
  connect(button, SIGNAL(clicked()), this, member);
  return button;
}
QString Model::op() const { return op_; }

void Model::setOp(const QString &op) { op_ = op; }

double Model::rhs() const { return rhs_; }

void Model::setRhs(double rhs) { rhs_ = rhs; }

double Model::lhs() const { return lhs_; }

void Model::setLhs(double lhs) { lhs_ = lhs; }

Model::State Model::state() const { return state_; }

void Model::setState(const Model::State &state) { state_ = state; }

QString Model::result() const { return result_; }

void Model::setResult(const QString &result) {
  if (result_ != result) {
    result_ = result;
    emit resultChanged(result_);
  }
}

Model defModel;


GuiInterface::GuiInterface(QWidget *parent, Model *model)
    : QWidget(parent), model_(model) {
  expression = new QLineEdit("");

  expression->setReadOnly(true);
  expression->setAlignment(Qt::AlignRight);
  // expression->setMaxLength(15);

  recentInput = new QLineEdit("0");

  recentInput->setReadOnly(true);
  recentInput->setAlignment(Qt::AlignRight);
  recentInput->setMaxLength(15);

  QFont font = recentInput->font();
  font.setPointSize(font.pointSize() + 8);
  recentInput->setFont(font);

  for (int i = 0; i < NumDigitButtons; ++i)
    digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

  Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
  Button *changeSignButton =
      createButton(tr("\302\261"), SLOT(changeSignClicked()));

  Button *backspaceButton =
      createButton(tr("Backspace"), SLOT(backspaceClicked()));
  Button *clearButton = createButton(tr("Clear"), SLOT(clear()));
  Button *clearAllButton = createButton(tr("Clear All"), SLOT(clearAll()));

  Button *clearMemoryButton = createButton(tr("MC"), SLOT(clearMemory()));
  Button *readMemoryButton = createButton(tr("MR"), SLOT(readMemory()));
  Button *setMemoryButton = createButton(tr("MS"), SLOT(setMemory()));
  Button *addToMemoryButton = createButton(tr("M+"), SLOT(addToMemory()));

  Button *divisionButton =
      createButton(tr("\303\267"), SLOT(operatorClicked()));
  Button *timesButton = createButton(tr("\303\227"), SLOT(operatorClicked()));
  Button *minusButton = createButton(tr("-"), SLOT(operatorClicked()));
  Button *plusButton = createButton(tr("+"), SLOT(operatorClicked()));

  Button *squareRootButton =
      createButton(tr("\u221a"), SLOT(operatorClicked()));  // 0xE2 0x88 0x9A
  Button *powerButton = createButton(tr("x\302\262"), SLOT(operatorClicked()));
  Button *reciprocalButton = createButton(tr("1/x"), SLOT(operatorClicked()));

  Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

  QGridLayout *mainLayout = new QGridLayout;

  mainLayout->setSizeConstraint(QLayout::SetFixedSize);

  mainLayout->addWidget(expression, 0, 0, 1, 6);
  mainLayout->addWidget(recentInput, 1, 0, 1, 6);

  mainLayout->addWidget(backspaceButton, 2, 0, 1, 2);
  mainLayout->addWidget(clearButton, 2, 2, 1, 2);
  mainLayout->addWidget(clearAllButton, 2, 4, 1, 2);

  mainLayout->addWidget(clearMemoryButton, 3, 0);
  mainLayout->addWidget(readMemoryButton, 4, 0);
  mainLayout->addWidget(setMemoryButton, 5, 0);
  mainLayout->addWidget(addToMemoryButton, 6, 0);

  for (int i = 1; i < NumDigitButtons; ++i) {
    int row = ((9 - i) / 3) + 3;
    int column = ((i - 1) % 3) + 1;
    mainLayout->addWidget(digitButtons[i], row, column);
  }

  mainLayout->addWidget(digitButtons[0], 6, 1);
  mainLayout->addWidget(pointButton, 6, 2);
  mainLayout->addWidget(changeSignButton, 6, 3);

  mainLayout->addWidget(divisionButton, 3, 4);
  mainLayout->addWidget(timesButton, 4, 4);
  mainLayout->addWidget(minusButton, 5, 4);
  mainLayout->addWidget(plusButton, 6, 4);

  mainLayout->addWidget(squareRootButton, 3, 5);
  mainLayout->addWidget(powerButton, 4, 5);
  mainLayout->addWidget(reciprocalButton, 5, 5);
  mainLayout->addWidget(equalButton, 6, 5);
  setLayout(mainLayout);

  setWindowTitle(tr("Calculator"));

  connect(this->model_, &Model::resultChanged, recentInput,
          &QLineEdit::setText);
}

//        NUM input
void GuiInterface::pointClicked() {
  auto r = model_->result();
  if (!r.contains('.')) {
    model_->setResult(r + '.');
    if (model_->state() == Model::State::WAITING_LHS)
      model_->setState(Model::State::LHS_INPUTTING);
    else if (model_->state() == Model::State::WAITING_RHS)
      model_->setState(Model::State::RHS_INPUTTING);
  }
}

void GuiInterface::changeSignClicked() {
  auto r = model_->result();
  if (r != "0") {
    if (r.startsWith('-')) {
      model_->setResult(r.remove(0, 1));
    } else {
      model_->setResult(r.insert(0, '-'));
    }
  }
}

void GuiInterface::digitClicked() {
  Button *clickedButton = qobject_cast<Button *>(sender());
  auto digit = clickedButton->text();

  switch (model_->state()) {
    case Model::State::WAITING_LHS:
      if (digit != '0') {
        model_->setResult(digit);
        model_->setState(Model::State::LHS_INPUTTING);
      }
      break;
    case Model::State::WAITING_RHS:
      if (digit != '0') {
        model_->setResult(digit);
        model_->setState(Model::State::RHS_INPUTTING);
      }
      break;
    case Model::State::LHS_INPUTTING:
    case Model::State::RHS_INPUTTING:
      model_->setResult(model_->result() + digit);
      break;
    default:
      break;
  }
}
//        Operator input
bool Model::checkOpPrecondition(const QString &op, double operand)  //
{
  if ((op == tr("\u221a") && operand < 0.0) ||
      (op == tr("1/x") && operand == 0.0) ||
      (op == tr("\303\267") && operand == 0.0)) {
    return false;
  } else {
    return true;
  }
}

double Model::doUnaryCalculation(const QString &clickedOperator,
                                        double operand) {
  double result = HUGE_VAL;

  if (clickedOperator == tr("\u221a")) {
    result = std::sqrt(operand);
  } else if (clickedOperator == tr("x\302\262")) {
    result = std::pow(operand, 2.0);
  } else if (clickedOperator == tr("1/x")) {
    result = 1.0 / operand;
  }

  setResult(QString::number(result));
  if (state() == Model::State::LHS_INPUTTING) {
    setLhs(result);
  } else if (state() == Model::State::RHS_INPUTTING) {
    setRhs(result);
  }

  return result;
}

double Model::doBinaryCalculation(double operand) {
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

bool Model::isUnaryOp(const QString &op) const {
  return op == tr("\u221a") || op == tr("1/x") || op == tr("x\302\262");
}

bool Model::isBinaryOp(const QString &op) const {
  return op == tr("\303\267") || op == "\303\227" || op == tr("-") ||
         op == tr("+");
}

void GuiInterface::operatorClicked() {
  // operand complete, result->lhs or rhs conversion
  bool ok;
  auto conversion = model_->result().toDouble(&ok);

  Button *clickedButton = qobject_cast<Button *>(sender());
  QString clickedOperator = clickedButton->text();

  if (model_->isBinaryOp(clickedOperator)) {
    if (ok) {
      switch (model_->state()) {
        case Model::State::WAITING_LHS:
        case Model::State::WAITING_RHS:
          break;
        case Model::State::LHS_INPUTTING:
          model_->setLhs(conversion);
          expression->setText(expression->text() + model_->result() +
                              clickedOperator);
          break;
        case Model::State::RHS_INPUTTING:
          model_->setRhs(conversion);
          expression->setText(expression->text() + model_->result() +
                              clickedOperator);
          model_->doBinaryCalculation(conversion);
          break;
        default:
          break;
      }

      model_->setOp(clickedOperator);
      model_->setState(Model::State::WAITING_RHS);
    }

  } else if (model_->isUnaryOp(clickedOperator)) {
    model_->doUnaryCalculation(clickedOperator, conversion);
  }
}

void GuiInterface::equalClicked() {
  if (model_->state() == Model::State::RHS_INPUTTING) {
    bool ok;
    auto conversion = model_->result().toDouble(&ok);

    if (model_->isBinaryOp(model_->op())) {
      model_->doBinaryCalculation(conversion);

      model_->setState(Model::State::WAITING_LHS);

    } else {
    }
  } else if (model_->state() == Model::State::WAITING_LHS) {
    model_->doBinaryCalculation(model_->rhs());
  }

  expression->clear();
}

void GuiInterface::backspaceClicked() {
  auto r = model_->result();
  auto bound = r.startsWith('-') ? 2 : 1;

  if (bound < r.size()) {
    r.chop(1);
    model_->setResult(r);
  } else {
    model_->setResult("0");
    if (model_->state() == Model::State::LHS_INPUTTING)
      model_->setState(Model::State::WAITING_LHS);
    else if (model_->state() == Model::State::RHS_INPUTTING)
      model_->setState(Model::State::WAITING_RHS);
  }
}

void GuiInterface::clear() {
  expression->clear();
  model_->setResult("0");
  switch (model_->state()) {
    case Model::State::WAITING_LHS:
    case Model::State::WAITING_RHS:
      break;
    case Model::State::LHS_INPUTTING:
      model_->setLhs(0.0);
      model_->setState(Model::State::WAITING_LHS);
      break;
    case Model::State::RHS_INPUTTING:
      model_->setRhs(HUGE_VAL);
      model_->setState(Model::State::WAITING_RHS);
      break;
    default:
      break;
  }
}

void GuiInterface::clearAll() {
  expression->clear();
  model_->setResult("0");
  model_->setLhs(0.0);
  model_->setRhs(HUGE_VAL);
  model_->setOp("");
  model_->setState(Model::State::WAITING_LHS);
}

void GuiInterface::clearMemory() {}
void GuiInterface::readMemory() {}
void GuiInterface::setMemory() {}
void GuiInterface::addToMemory() {}

#include "model.h"

Model defModel;

static QString ops[] = {QObject::tr("+"),
                        QObject::tr("-"),
                        QObject::tr("\303\227"),
                        QObject::tr("\303\267"),
                        QObject::tr("%"),
                        QObject::tr("\302\261"),
                        QObject::tr("x\302\262"),
                        QObject::tr("1/x"),
                        QObject::tr("\u221a"),
                        QObject::tr("=")
};
constexpr auto ops_size = sizeof(ops)/sizeof(ops[0]);

QString to_string (Calculator::Operator val) {
    return ops[val];
}

Calculator::Operator to_operator(const QString& val)
{
    auto end = ops + ops_size;
    auto it = std::find(ops, end, val);
    if (it != end)
        return Calculator::Operator(std::distance(ops, it));
    else
        return Calculator::Operator::Unknown;
}

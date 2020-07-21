#include "model.h"

Model defModel;


QString to_string (Calculator::Operator val) {
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
    return ops[val];
}

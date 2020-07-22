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

QString to_string (Operator val) {
    return ops[val];
}

Operator to_operator(const QString& val)
{
    auto end = ops + ops_size;
    auto it = std::find(ops, end, val);
    if (it != end)
        return Operator(std::distance(ops, it));
    else
        return Operator::UNKNOWN_OPERATOR;
}

#include <QCoreApplication>
#include "Stack.hpp"

using namespace calculator::model;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Stack<double> model;
    model.push(1.0);
    model.push(2.0);
    model.push(3.0);

    auto elements = model.getElements(1);

    model.swapTop2();

    auto elements_2 = model.getElements(1);

    return a.exec();
}

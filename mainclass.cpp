#include "mainclass.h"
#include "data.h"
#include "install.h"
#include "log.h"
#include <QApplication>
#include <QErrorMessage>
#include <QString>

int mainclass::exec(int argc, char **argv) {
    options set;
    install ins;
    QApplication app(argc,argv);
    ins.read();
    (new Window(&ins, set.read_set(false), &set))->show();
    log::message(0, __FILE__, __LINE__, "Window exec");
    int res = app.exec();
    log::message(0, __FILE__, __LINE__, "Window closed");
    set.autowrite_set();
    ins.write();
    log::message(0, __FILE__, __LINE__, QString("Exiting... Returned ") + QString::number(res));
    return res;
}


















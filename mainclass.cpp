#include "mainclass.h"
#include "data.h"
#include "log.h"
#include <QApplication>
#include <QErrorMessage>
#include <QString>

int mainclass::exec(int argc, char **argv) {
    options set;
    QApplication app(argc,argv);
    (new Window(set.read_set(false), &set))->show();
    int res = app.exec();
    set.autowrite_set();
    log::message(0, __FILE__, __LINE__, QString("Exiting... Returned ") + char(res + 48));
    return res;
}


















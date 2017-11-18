#include "data.h"
#include "window.h"
#include "install.h"
#include <version.h>
#include "log.h"
#include "cmd.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>

const QString VERSION = VER_PRODUCTVERSION_STR;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QString workDir;
    workDir = app.applicationDirPath();
    log::message(0, __FILE__, __LINE__, QString("Work dir is ") + workDir);
    options set;
    install ins(workDir, &set);
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

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
static QString workDir;
const QString &WORK_DIR = workDir;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    workDir = app.applicationDirPath();
    cmd::exec("help");
    LOG(0, QString("Work dir is ") + WORK_DIR);
    options set;
    install ins(&set);
    ins.read();
    (new Window(&ins, set.read_set(false), &set))->show();
    LOG(0, "Window exec");
    int res = app.exec();
    LOG(0, "Window closed");
    set.autowrite_set();
    ins.write();
    LOG(0, QString("Exiting... Returned ") + QString::number(res));
    return res;
}

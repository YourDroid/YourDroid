#include "data.h"
#include "window.h"
#include "install.h"
#include <version.h>
#include "log.h"
#include "cmd.h"
#include "console.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QDebug>

const QString VERSION = VER_PRODUCTVERSION_STR;
static QString workDir;
const QString &WORK_DIR = workDir;
console *log::con;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    qInstallMessageHandler(log::messagenew);
    console *widget = log::init(0);
    options set;
    bool readSet = set.read_set(false);
    if(argc == 2 && argv[1] == "c" || set.getConEnable()) log::setEnabledCon(true);
    workDir = app.applicationDirPath();
    cmd::exec("help");
    LOG(0, QString("Work dir is ") + WORK_DIR);
    qDebug() << "fiest valid mes!";
    install ins(&set);
    ins.read();
    Window *window = new Window(&app, &ins, readSet, &set);
    window->show();
    QObject::connect(window, &Window::closed, [=](){ widget->close(); });
    QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });
    LOG(0, "Window exec");
    int res = app.exec();
    LOG(0, "Window closed");
    set.autowrite_set();
    ins.write();
    LOG(0, QString("Exiting... Returned ") + QString::number(res));
    return res;
}

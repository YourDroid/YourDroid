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

const QString VERSION = VER_PRODUCTVERSION_STR;
static QString workDir;
const QString &WORK_DIR = workDir;
console *log::con;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    workDir = app.applicationDirPath();
    qInstallMessageHandler(log::messagenew);
#ifdef Q_OS_LINUX
    if(!QFile().exists(QString::fromLocal8Bit(qgetenv("HOME")) + "/.config/QtProject/qtlogging.ini"))
        system("touch ~/.config/QtProject/qtlogging.ini");
#endif
    console *widget = log::init(0);
    options set;
    bool readSet = set.read_set(false);
    QTranslator translator;
    translator.load("yourdroid_" + QString::fromStdString(_langHelper::to_string(set.getLang())));
    app.installTranslator(&translator);
    if(argc == 2 && argv[1] == "c" || set.getConEnable()) log::setEnabledCon(true);
    cmd::exec("help");
    qDebug() << QString(app.translate("log", "Work dir is ")) + WORK_DIR;
    install ins(&set);
    ins.read();
    Window *window = new Window(&ins, readSet, &set);
    window->show();
    QObject::connect(window, &Window::closed, [=](){ widget->close(); });
    QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });
    qDebug() << app.translate("log", "Window exec");
    int res = app.exec();
    qDebug() << app.translate("log", "Window closed");
    set.autowrite_set();
    ins.write();
    qDebug() << app.translate("log", "Exiting... Returned ") << QString::number(res);
    return res;
}

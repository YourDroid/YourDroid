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
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>

#if WIN
#include <exception>
#include <VirtDisk.h>
#include <windows.h>
#endif

const QString VERSION = VER_PRODUCTVERSION_STR;
static QString workDir;
const QString &WORK_DIR = workDir;
console *log::con;

void segFault(int res) {
    qCritical() << QObject::tr("^Segmentation Fault");
    char buffer[1024];
    backtrace_symbols(buffer, 1024);
    qDebug() << buffer;
    signal(res, SIG_DFL);
    exit(3);
}

int main(int argc, char *argv[])
{
    std::freopen("./log/stderr.txt", "a+", stderr);
    fprintf(stderr, "\n\n###NEW###");
    int *h;
    h = 0;
    *h = 9;

    try {
        QApplication app(argc,argv);
        qRegisterMetaType<QtMsgType>("QtMsgType");
        workDir = app.applicationDirPath();
        qInstallMessageHandler(log::messagenew);
#if LINUX
        if(!QFile().exists(QString::fromLocal8Bit(qgetenv("HOME")) + "/.config/QtProject/qtlogging.ini"))
            system("touch ~/.config/QtProject/qtlogging.ini");
#endif
        console *widget = log::init();
        signal(SIGSEGV, segFault);
#if LINUX
        int uid = geteuid();
        qDebug().noquote() << QObject::tr("getuid() returned %1").arg(uid);
        if(uid != 0) {
            if(!QProcess::startDetached(QString("gksudo %1").arg(qApp->arguments()[0]))) {
                qCritical() << QObject::tr("^Program must be run with root. Run \'sudo %1\' in the terminal to fix it")
                               .arg(qApp->applicationFilePath());
                return 1;
            }
            return 0;
        }
#endif
        options set;
        bool readSet = set.read_set(false);

        QTranslator translator;
        translator.load("yourdroid_" + QString::fromStdString(_langHelper::to_string(set.getLang())));
        app.installTranslator(&translator);
        if(argc == 2 && argv[1] == "c" || set.getConEnable()) log::setEnabledCon(true);

        cmd::exec("help");

        qDebug().noquote() << QString(app.translate("log", "Work dir is ")) + WORK_DIR;
        install ins(&set);
        ins.read();

        Window *window = new Window(&ins, readSet, &set);
        window->show();
        QObject::connect(window, &Window::closed, [=](){ widget->close(); });
        QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });

        qDebug().noquote() << app.translate("log", "Window exec");
        int res = app.exec();
        qDebug().noquote() << app.translate("log", "Window closed");
        set.autowrite_set();
        ins.write();
        qDebug().noquote() << QObject::tr("Exiting... Returned ") << QString::number(res);
        return res;
    }
    catch(...) {
        qDebug().noquote() << "jkfghgkdfj";
            return -1;
    }
}

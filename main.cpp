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
#include <signal.h>
#include "exception.h"
#include <QtConcurrent/QtConcurrentRun>

#if WIN
#include <windows.h>
#endif

#if !WIN && !LINUX
#error This system does not support
#endif

const QString VERSION = VER_PRODUCTVERSION_STR;
console *log::con;

int main(int argc, char *argv[])
{
    qDebug() << QString::number(getpid()).prepend('^');
    std::freopen("./log/stderr.txt", "a+", stderr);
    fprintf(stderr, "\n\n###NEW###");
#if LINUX
    gdb_SetProgName(argv[0]);
#endif
    set_signal_handler();
    std::set_terminate([=](){
        qCritical().noquote().noquote() << QObject::tr("^Unknown fatal error! Program will terminate");
        errorAbort(1);
    });

    try {
        QApplication app(argc,argv);
        qRegisterMetaType<QtMsgType>("QtMsgType");
        qRegisterMetaType<QTextCursor>("QTextCursor");
        qRegisterMetaType<QTextBlock>("QTextBlock");
        qInstallMessageHandler(log::message);
#if LINUX
        if(!QFile().exists(QString::fromLocal8Bit(qgetenv("HOME")) + "/.config/QtProject/qtlogging.ini"))
            system("touch ~/.config/QtProject/qtlogging.ini");
#endif
        //console *widget = log::init();
#if LINUX
        int uid = geteuid();
        qDebug().noquote() << QObject::tr("getuid() returned %1").arg(uid);
        if(uid != 0) {
            qCritical().noquote() << QObject::tr("^Program must be run with root. "
                                                 "Press ok to try run it as root. "
                                                 "Or press cancel to exit and run \'sudo %1\' in the terminal to fix it|+-|")
                                     .arg(qApp->applicationFilePath());
            switch(log::getLastPressedButton()) {
            case QMessageBox::Ok: if(!QProcess::startDetached(QString("gksudo %1").arg(argv[0]))) {
                    qCritical().noquote() << QObject::tr("^Could not run it as root. "
                                                         "Please run \'sudo %1\' in the terminal to fix it")
                                             .arg(qApp->applicationFilePath());
                    return 1;
                } break;
            case QMessageBox::Cancel: return 1;
            }
            return 0;
        }
#endif
        options set;
        bool readSet = set.read_set(false);

        qDebug().noquote() << QObject::tr("Translating...");
        QTranslator translator;
        translator.load("yourdroid_" + QString::fromStdString(_langHelper::to_string(set.getLang())));
        app.installTranslator(&translator);

        qDebug().noquote() << QObject::tr("Setting debug console...");
        //if(argc == 2 && argv[1] == "c" || set.getConEnable()) log::setEnabledCon(true);

        cmd::exec("help");
        QtConcurrent::run([=](){*(int*)0 = 0;});

        install ins(&set);
        ins.read();

        Window *window = new Window(&ins, readSet, &set);
        window->show();
//        QObject::connect(window, &Window::closed, [=](){ widget->close(); });
//        QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });

        qDebug().noquote() << app.translate("log", "Window exec");
        int res = app.exec();
        qDebug().noquote() << app.translate("log", "Window closed");
        set.autowrite_set();
        ins.write();
        return 8;
        qDebug().noquote() << QObject::tr("Exiting... Returned ") << QString::number(res);
        return res;
    }
    catch(std::exception ex) {
        qCritical().noquote().noquote() << QObject::tr("^Fatal error: %1").arg(QString::fromStdString(ex.what()));
        errorAbort(1);
    }
    catch(...) {
        qCritical().noquote().noquote() << QObject::tr("^Unknown fatal error!");
        errorAbort(1);
    }
}

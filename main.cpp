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

#if WIN
#include <exception>
#include <VirtDisk.h>
#include <windows.h>
#elif LINUX
#include <err.h>
#endif

const QString VERSION = VER_PRODUCTVERSION_STR;
static QString workDir;
const QString &WORK_DIR = workDir;
console *log::con;

#if WIN
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
{
  qCritical().noquote() <<
                           QObject::tr("Fatal error! Here is some info about it: exception code: %1")
                           .arg((int)ExceptionInfo->ExceptionRecord->ExceptionCode);
  exit(3);
  return EXCEPTION_EXECUTE_HANDLER;
}
#elif LINUX
void posix_signal_handler(int sig, siginfo_t *siginfo, void *context)
{
    qCritical().noquote() <<
                             QObject::tr("Fatal error! Here is some info about it: exception code: %1; exception info code %2")
                             .arg(sig, siginfo->si_code);
    exit(3);
}
#endif


void set_signal_handler()
{
#if WIN
SetUnhandledExceptionFilter(windows_exception_handler);
#elif LINUX
  {
    struct sigaction sig_action = {};
    sig_action.sa_sigaction = posix_signal_handler;
    sigemptyset(&sig_action.sa_mask);
 
    #ifdef __APPLE__
        /* for some reason we backtrace() doesn't work on osx
           when we use an alternate stack */
        sig_action.sa_flags = SA_SIGINFO;
    #else
        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    #endif
 
    if (sigaction(SIGSEGV, &sig_action, NULL) != 0) { err(1, "sigaction"); }
    if (sigaction(SIGFPE,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
    if (sigaction(SIGINT,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
    if (sigaction(SIGILL,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
    if (sigaction(SIGTERM, &sig_action, NULL) != 0) { err(1, "sigaction"); }
    if (sigaction(SIGABRT, &sig_action, NULL) != 0) { err(1, "sigaction"); }
  }
  #endif
}

int main(int argc, char *argv[])
{
    std::freopen("./log/stderr.txt", "a+", stderr);
    fprintf(stderr, "\n\n###NEW###");
    set_signal_handler();

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
    catch(std::exception ex) {
        qCritical().noquote() << QObject::tr("Fatal error: %1").arg(QString::fromStdString(ex.what()));
        return 1;
    }
    catch(...) {
        qCritical().noquote() << QObject::tr("Unknown fatal error!");
        return 1;
    }
}
#include "options.h"
#include "window.h"
#include "install.h"
#include <version.h>
#include "log.h"
#include "cmd.h"
#include "console.h"
#include "downloader.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDir>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include "exception.h"
#include <QtConcurrent/QtConcurrentRun>
#include "global.h"
#include <QSettings>

#if WIN
#include <windows.h>
#endif

#if !WIN && !LINUX
#error This system is not supported
#endif

const _global *global;
console *log::con;

int main(int argc, char *argv[])
{
    //qDebug() << QString::number(getpid()).prepend('^');
    system("mkdir ./log");
    FILE *stderrTxt = 0;
    bool fork = false;
    if(argc >= 2)
    {
        puts("There is an argument: ");
        puts(argv[1]);
        if(QString(argv[1]) == "-fork")
        {
            fork = true;
            puts("This is a fork. Redirecting stderr to stderr_.txt");
            stderrTxt = std::freopen("./log/stderr_.txt", "a+", stderr);

            if(argc >= 3)
            {
                puts("Changing working directory to:");
                puts(argv[2]);
                if(QDir::setCurrent(argv[2]))
                    puts("Success");
                else puts("Failed");
            }
        }
    }
    if(!fork) stderrTxt = std::freopen("./log/stderr.txt", "a+", stderr);
    fprintf(stderr, "\n\n###NEW###\n");

    //set_signal_handler();
//    std::set_terminate([=]() -> void {
//        qCritical().noquote().noquote() << QObject::tr("^Unknown fatal error! Program will terminate");
//        errorAbort(1);
//    });
    auto exceptionAbort = [&](QString what) {
        qCritical().noquote() << QObject::tr("^Fatal error: %1").arg(what);
        //errorAbort(1);
    };

    try {
        QApplication app(argc,argv);
#if LINUX
        bool globalRunAsAppimage = QFile(QCoreApplication::applicationDirPath() + "/run_as_appimage").exists();
#endif

        qRegisterMetaType<QtMsgType>("QtMsgType");
        qRegisterMetaType<QTextCursor>("QTextCursor");
        qRegisterMetaType<QTextBlock>("QTextBlock");
        qInstallMessageHandler(log::message);

        log::init();
#if LINUX
        if(!QFile().exists(QString::fromLocal8Bit(qgetenv("HOME")) + "/.config/QtProject/qtlogging.ini"))
            system("touch ~/.config/QtProject/qtlogging.ini");

        system("echo $APPIMAGE");

        int uid = geteuid();
        int ret = 0;
        qDebug().noquote() << QObject::tr("getuid() returned %1").arg(uid);
        if(uid != 0) {
            qDebug().noquote() << "Executing itself as root...";
            fflush(stderr);
            fclose(stderrTxt);

            QString workDir;
            if(QFile(QCoreApplication::applicationDirPath() + "/run_as_appimage").exists())
            {
                workDir = "./";
            }
            else
            {
                workDir = globalGetWorkDir() + "/";
            }
            workDir = QFileInfo(workDir).absolutePath();
            qDebug().noquote() << "Working dir of a fork:" << workDir;

            QString command;
            if(globalRunAsAppimage)
            {
                command = "pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY $APPIMAGE -fork \"" + workDir + "\"";
            }
            else
            {
                command = "pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY " + qApp->applicationFilePath()
                        + " -fork \"" + workDir + "\"";
            }

            qDebug().noquote() << command.toStdString().c_str();

            system(command.toStdString().c_str());
            qDebug().noquote() << ret;

            if(ret)
            {
                qCritical().noquote() << QObject::tr("^Program must be run with root.");
                return 1;
            }
            return 0;
        }
#endif
        options set;
        qDebug().noquote() << "########## System specifications ##########";
        qDebug().noquote() << "# OS name - " << QOperatingSystemVersion::current().name();
        qDebug().noquote() << "# OS version - Major: " << QOperatingSystemVersion::current().majorVersion()
                        << " Minor: " << QOperatingSystemVersion::current().minorVersion()
                        << " Micro: " << QOperatingSystemVersion::current().microVersion();
        qDebug().noquote() << "# Working directory:" << globalGetWorkDir();
        set.defarch();
        set.defbios();
        auto screenSize = QGuiApplication::primaryScreen()->geometry();
        qDebug().noquote() << "# Screen resolution: " << screenSize.height()
                           << "x" << screenSize.width();
        qDebug().noquote() << "# Supports SSL:" << QSslSocket::supportsSsl();
        qDebug().noquote() << "# OpenSSL version:" << QSslSocket::sslLibraryBuildVersionString()
                           << QSslSocket::sslLibraryVersionString();
        qDebug().noquote() << "###########################################";
        bool readSet = set.read_set(false);

        qDebug().noquote() << QObject::tr("Translating...");
        QTranslator translator;
        translator.load("yourdroid_" + QString::fromStdString(_langHelper::to_string(set.getLang())));
        app.installTranslator(&translator);

        //qDebug().noquote() << QObject::tr("Setting debug console...");
        //if((argc == 2 && QString(argv[1]) == "c")) log::setEnabledCon(true);
//        qDebug().noquote() << "fix";

//        *(int*)0 = 0;
#if WIN
        set.mountEfiPart();
#endif

        install ins(&set);
        ins.read();

        global = new _global(&set, &ins);

        Window *window = new Window(readSet);
        //global->insSet->downloadImage(QUrl("https://raw.githubusercontent.com/YourDroid/Info/master/android_list.ini"));
//        Downloader downloader;
//        downloader.get(globalWorkDir() + "/andra.iso", QUrl("https://dotsrc.dl.osdn.net/osdn/android-x86/71931/android-x86-9.0-r1.iso"));
        window->show();
        //        QObject::connect(window, &Window::closed, [=](){ widget->close(); });
        //        QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });

        qDebug().noquote() << app.translate("log", "Window exec");
        int res = app.exec();
        qDebug().noquote() << app.translate("log", "Window closed");
        set.autowrite_set();
        //ins.write();
#if WIN
        set.unmountEfiPart();
#endif
        qDebug().noquote() << QObject::tr("Exiting... Returned ") << QString::number(res);
        return res;
    }
    catch(std::exception ex) {
        exceptionAbort(ex.what());
    }
    catch(QException ex) {
        exceptionAbort(ex.what());
    }
    catch(...) {
        exceptionAbort(QObject::tr("Unknown error"));
    }
}

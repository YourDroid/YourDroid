#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>
#include <QtGlobal>
#include <QProcess>

#if WIN
#include <windows.h>
#include <winbase.h>
#include <w32api.h>
#endif

QPair<int, QString> cmd::exec(QString command, QStringList list, bool disFsRedir) {
#if WIN
    static BOOL archOs = false;
    if(disFsRedir) {
        IsWow64Process(GetCurrentProcess(), &archOs);

        if(archOs) {
            qDebug().noquote() << QObject::tr("Disabling fs redirection...");
            if(!Wow64EnableWow64FsRedirection(FALSE))
                qWarning().noquote() << QObject::tr("Could not disable fs redirection");
        }
    }
#endif

    qDebug().noquote() << QObject::tr("Executing \"%1\"").arg(command);
    QString _output;
    QProcess proc;
    bool succes = true, started = true;

    if(list.isEmpty()) proc.start(command);
    else proc.start(command, list);

    int _res = 22;
    if(!proc.waitForStarted()) {
        succes = false;
        started = false;
    }
    else if(!proc.waitForFinished(-1)) {
        succes = false;
    }
    else if(proc.exitStatus() == QProcess::CrashExit) {
        succes = false;
        qCritical().noquote() << (_output = QObject::tr("Application has crashed"));
    }
    if(succes) qDebug().noquote() << QObject::tr("Executing ended succesfull");
    else {
        _output = QTextCodec::codecForName("CP1251")->toUnicode(
                    proc.errorString().toLocal8Bit()).prepend("\n");
        qCritical().noquote() << QObject::tr("Executing ended unsuccesfull");
    }

    if(started) {
        QTextCodec *codec = QTextCodec::codecForName("CP866");
        QString tmp = codec->toUnicode(proc.readAll());
        _output = QString(tmp.isEmpty() ? "" : tmp) + _output;
        _res = (proc.exitStatus() == QProcess::CrashExit) ? 1 : proc.exitCode();
        if(_res) {
            qWarning().noquote() << QObject::tr("Returned value is ") << _res;
        }
        else {
            qDebug().noquote() << QObject::tr("Returned value is ") << _res;
        }
    }
    if(!_output.isEmpty()) qDebug().noquote() << _output;

#if WIN
    if(disFsRedir) {
        if(archOs) {
            qDebug().noquote() << QObject::tr("Enabling fs redirection...");
            if(!Wow64EnableWow64FsRedirection(TRUE))
                qWarning().noquote() << QObject::tr("Could not enable fs redirection");
        }
    }
#endif

    return QPair<int, QString>(_res, _output);
}

















#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>
#include <QtGlobal>
#include <QProcess>
#if WIN
#include <windows.h>
//#include <Processthreadsapi.h>
#include <winbase.h>
//#include <TCHAR.H>
#include <w32api.h>
#endif

QString convert(char *str, char *from, char *to)
{
QTextCodec* codec = QTextCodec::codecForName(from);
if (!codec) return NULL;
QString unicodeString = codec->toUnicode( str);

if (strcmp(from,to))
{
codec = QTextCodec::codecForName(to);
if (!codec) return NULL;

return codec->fromUnicode(unicodeString);
}

return unicodeString;
}

QPair<int, QString> cmd::exec(QString command) {
    qDebug().noquote() << QObject::tr("Executing \"%1\"").arg(command);
    QString _output = "";
    QProcess proc;
    bool succes = true, started = true;
    proc.start(command);
    int _res = 22;
    if(!proc.waitForStarted()) {
        qCritical() << (_output = QObject::tr("Command could not start"));
        succes = false;
        started = false;
    }
    else if(!proc.waitForFinished()) {
        qCritical() << (_output = QObject::tr("Command could not finish"));
        succes = false;
        started = false;
    }
    else if(proc.exitStatus() == QProcess::CrashExit) {
        succes = false;
        qCritical() << (_output = QObject::tr("Application has crashed"));
    }
    if(succes) qDebug().noquote() << QObject::tr("Executing ended succesfull");
    else qCritical() << QObject::tr("Executing ended unsuccesfull");

    if(started) {
        QTextCodec *codec = QTextCodec::codecForName("CP866");
        QString tmp = codec->toUnicode(proc.readAll());
        _output = QString(tmp.isEmpty() ? "" : tmp) + _output;
        _res = (proc.exitStatus() == QProcess::CrashExit) ? 1 : proc.exitCode();
        qDebug().noquote() << _output;
        if(_res) {
            qWarning() << QObject::tr("Returned value is ") << _res;
        }
        else {
            qDebug().noquote() << QObject::tr("Returned value is ") << _res;
        }
    }

    return QPair<int, QString>(_res, _output);
}

















#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>

QPair<int, QString> cmd::exec(QString command) {
    qDebug() << qApp->translate("log", "Executing ") + command;
    QString _output = "";
    int _res;
//#if OS == 1
//    QString home = getenv("USERPROFILE");
//    QString strcmd = command + qApp->translate("log", ">") + home + qApp->translate("log", "\\temp_cmd 2>&1");
//    QFile tempCmd(home + qApp->translate("log", "\\temp_cmd"));
//#elif OS == 0
//    QString home = getenv("home");
//    QString strcmd = command + qApp->translate("log", " &> ") + home + qApp->translate("log", "/temp_cmd");
//    QFile tempCmd(home + qApp->translate("log", "/temp_cmd"));
//#endif
//    _res = system(strcmd.toStdString().c_str());
//    if (tempCmd.open(QIODevice::ReadOnly))
//    {
//        _output = tempCmd.readAll();
//        tempCmd.close();
//    }
#if LINUX
    FILE *trm = popen((command + QString(" 2>&1")).toStdString().c_str(), "r");
    char buffer[128];
    while(!feof(trm)) {
         if(fgets(buffer, 128, trm) != NULL)
             _output += buffer;
    }
    _res = pclose(trm);
#elif WIN
    PROCESS_INFORMATION p_info;
    STARTUPINFO s_info;
    LPSTR cmdline, programpath;

    memset(&s_info, 0, sizeof(s_info));
    memset(&p_info, 0, sizeof(p_info));
    s_info.cb = sizeof(s_info);

    cmdline     = _tcsdup(TEXT(cmd));
    programpath = _tcsdup(TEXT(cmd));

    _res = CreateProcess(programpath, cmdline, NULL, NULL, 0, 0, NULL, NULL, &s_info, &p_info);
    if(_res)
    {
      WaitForSingleObject(p_info.hProcess, INFINITE);
      CloseHandle(p_info.hProcess);
      CloseHandle(p_info.hThread);
    }
#endif
    if(_res) {
        qCritical() << _output;
        qCritical() << QObject::tr("Error while executing!");
    }
    else {
        qDebug() << _output;
        qDebug() << QObject::tr("Command executed succesful");
    }
    return QPair<int, QString>(_res, _output);
}

//QPair<int, QString> cmd::exec() {
//    return exec(_command);
//}

















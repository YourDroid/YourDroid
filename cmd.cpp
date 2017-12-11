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
    FILE *trm = popen((command + QString(" 2>&1")).toStdString().c_str(), "r");
    char tmp;
    //_output.clear();
    char buffer[128];
    while(!feof(trm)) {
         if(fgets(buffer, 128, trm) != NULL)
             _output += buffer;
    }
    while(!feof(trm)) while(fgets(&tmp, 1, trm)) {
        _output += tmp;
        if(tmp == '\n') break;
    }
    _res = pclose(trm);
    qDebug() << _output;
    if(_res) qCritical() << "Error while executing!";
    else qDebug() << qApp->translate("log", "Command executed succesful");
    return QPair<int, QString>(_res, _output);
}

//QPair<int, QString> cmd::exec() {
//    return exec(_command);
//}

















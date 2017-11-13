#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>

QPair<int, QString> cmd::exec(QString command) {
    LOG(0, QString("Executing ") + command);
//#if OS == 1
//    QString home = getenv("USERPROFILE");
//    QString strcmd = command + QString(">") + home + QString("\\temp_cmd 2>&1");
//    QFile tempCmd(home + QString("\\temp_cmd"));
//#elif OS == 0
//    QString home = getenv("home");
//    QString strcmd = command + QString(" &> ") + home + QString("/temp_cmd");
//    QFile tempCmd(home + QString("/temp_cmd"));
//#endif
//    _res = system(strcmd.toStdString().c_str());
//    if (tempCmd.open(QIODevice::ReadOnly))
//    {
//        _output = tempCmd.readAll();
//        tempCmd.close();
//    }
    FILE *trm = popen(command.toStdString().c_str(), "r");
    char tmp;
    _output = "";
    while(!feof(trm)) while(fgets(&tmp, 1, trm)) {
        _output += tmp;
        if(tmp == '\n') break;
    }
    _res = pclose(trm);
    if(_res) LOG(2, "Error while executing!");
    return QPair<int, QString>(_res, _output);
}

int cmd::exec() {
    return exec(_command).first;
}

















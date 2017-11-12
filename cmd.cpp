#include "cmd.h"
#include <QFile>

int cmd::exec(QString command) {
#if OS == 1
    QString home = getenv("USERPROFILE");
    QString strcmd = command + QString(">") + home + QString("\\temp_cmd 2>&1");
    QFile tempCmd(home + QString("\\temp_cmd"));
#elif OS == 0
    QString home = getenv("home");
    QString strcmd = command + QString(" &> ") + home + QString("/temp_cmd");
    QFile tempCmd(home + QString("/temp_cmd"));
#endif
    _res = system(strcmd.toStdString().c_str());
    if (tempCmd.open(QIODevice::ReadOnly))
    {
        _output = tempCmd.readAll();
        tempCmd.close();
    }
    return _res;
}

int cmd::exec() {
    return exec(_command);
}

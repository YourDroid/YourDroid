#include "global.h"

QString globalGetWorkDir()
{
    static QDir dir = QCoreApplication::applicationDirPath();
    static bool first = true;
    if(QFile(QCoreApplication::applicationDirPath() + "/qt.conf").exists() && first)
    {
        dir.cdUp();
    }
    if(first)
    {
        qDebug().noquote() << dir.path();
        first = false;
    }
    return dir.path();
}

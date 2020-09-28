#include "global.h"

QString globalGetWorkDir()
{
    static QDir dir = QCoreApplication::applicationDirPath();
    static bool first = true;
    if(QFile(dir.path() + "/qt.conf").exists() &&
            !QFile(QCoreApplication::applicationDirPath() + "/run_as_appimage").exists() && first)
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

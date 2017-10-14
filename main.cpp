#include "mainclass.h"
#include "data.h"
#include "install.h"
#include <version.h>
#include "log.h"
#include <QApplication>
#include <QErrorMessage>
#include <QString>

const QString VERSION = VER_PRODUCTVERSION_STR;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QString workDir;
//#if OS == 0
//    FILE *fp = popen("echo $(cd $(dirname $0) && pwd)", "r");
//    char buf[256];
//    fgets(buf, sizeof(buf) - 1, fp);
//    pclose(fp);
//    workDir = buf;
//    workDir = workDir.left(workDir.indexOf('\n'));
//#elif OS == 1
//    workDir = argv[0];
//    QString temp = workDir;
//    for(int i = 0; i <= workDir.length(); i++) temp[i] = workDir[workDir.length() - i];
//    workDir = workDir.left(workDir.length() - temp.indexOf('\\'));
//#endif
    workDir = app.applicationDirPath();
    log::message(0, __FILE__, __LINE__, QString("Work dir is ") + workDir);
    options set;
    install ins(workDir, &set);
    ins.read();
    (new Window(&ins, set.read_set(false), &set))->show();
    log::message(0, __FILE__, __LINE__, "Window exec");
    int res = app.exec();
    log::message(0, __FILE__, __LINE__, "Window closed");
    set.autowrite_set();
    ins.write();
    log::message(0, __FILE__, __LINE__, QString("Exiting... Returned ") + QString::number(res));
    return res;
}

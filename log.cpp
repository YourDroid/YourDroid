#include <log.h>
#include <QErrorMessage>
#include "os.h"

//log::log(QString t) : typeName(t) { logFile.open("log.txt"); }

void log::message(int level, QString file, int line, QString mess, QString rusMess) {
    using namespace std;
    static ofstream logFile("log.txt");
    QString typeName;
    switch(level) {
    case 0: typeName = "DEBUG:"; break;
    case 1: typeName = "WARNING:"; break;
    case 2: typeName = "ERROR:"; break;
    }
#if OS == 0
    QString color;
    switch(level) {
    case 0: color = "\x1b[0m"; break;
    case 1: color = "\x1b[34m"; break;
    case 2: color = "\x1b[31m"; break;
    }
    QString messFull = color + typeName + QString(' ') + mess + "\x1b[0m";
#elif OS == 1
//    switch(level) {
//    case 1: system("color 09"); break;
//    case 2: system("color 0c"); break;
//    }
    QString messFull = typeName + QString(' ') + mess;
#endif
    logFile << "FILE: " << file.toStdString() << " LINE: " << line << " " << typeName.toStdString() << " " << mess.toStdString() << endl;
    cout << messFull.toStdString() << endl;
#if OS == 1
//    system("color 00");
#endif
    if(rusMess != "NULL") {
        switch(level) {
        case 0: typeName.clear(); break;
        case 1: typeName = "Внимание! "; break;
        case 2: typeName = "Ошибка! "; break;
        }
        QErrorMessage mess;
        mess.showMessage(typeName + rusMess);
        mess.exec();
    }
}

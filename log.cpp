#include <log.h>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDialog>
#include <QMessageBox>

//log::log(QString t) : typeName(t) { logFile.open("log.txt"); }

void log::message(int level, QString file, int line, QString mess, QString rusMess) {
    using namespace std;
    if(!QDir("log").exists()) QDir().mkdir("log");
    static QFile preLog(WORK_DIR + QString("/log/log-") + QDate::currentDate().toString("dMyy") + QTime::currentTime().toString("hhmmss") + QString(".txt"));
    static QTextStream logFile(&preLog);
    if(!preLog.isOpen()) {
        preLog.open(QIODevice::WriteOnly);
        logFile << "### Created by YourDroid " << VERSION << " ###" << endl << endl;
    }
    //static ofstream logFile("log.txt");
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
    logFile << "TIME:" << " " << QTime::currentTime().toString("hh:mm:ss") << " FILE: " << file << " LINE: " << line << " " << typeName << " " << mess << endl;
    cout << messFull.toStdString() << endl;
#if OS == 1
//    system("color 00");
#endif
    if(rusMess != "NULL") {
        switch(level) {
        case 0: QMessageBox::information(0, "Информация",
                                         rusMess,
                                         QMessageBox::Ok); break;
        case 1: QMessageBox::warning(0, "Внимание!",
                                     rusMess,
                                     QMessageBox::Ok); break;
        case 2: QMessageBox::critical(0, "Ошибка!",
                                      rusMess,
                                      QMessageBox::Ok); break;
        }
    }
}









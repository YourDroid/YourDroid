#include <log.h>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDialog>
#include <QMessageBox>

//log::log(QString t) : typeName(t) { logFile.open("log.txt"); }

console *log::init() {
    con = new console;
    con->setWindowTitle("YourDroid");
    return con;
}

void log::messagenew(QtMsgType level, const QMessageLogContext &context, const QString &message/*, QString rusMess*/) {
    bool window = false;
    QString mess = message;
    mess.remove('\"');
    if(mess[0] == '^') {
        window = true;
        mess.remove(0, 1);
    }
    using namespace std;
    if(!QDir("log").exists()) QDir().mkdir("log");
    static QString logName = qApp->applicationDirPath() + QString("/log/log-") + QDate::currentDate().toString("dMyy") + QTime::currentTime().toString("hhmmss") + ".txt";
    static QFile preLog(logName);
    static QTextStream logFile(&preLog);
    if(!preLog.isOpen()) {
        preLog.open(QIODevice::WriteOnly);
        logFile << "### Created by YourDroid " << VERSION << " ###" << endl << endl;
        std::freopen(logName.toStdString().c_str(), "a+", stderr);
    }
    //static ofstream logFile("log.txt");
    QString typeName;
    switch(level) {
    case 0: typeName = "DEBUG:"; break;
    case 1: typeName = "WARNING:"; break;
    case 2: typeName = "ERROR:"; break;
    case QtFatalMsg: typeName = "FATAL ERROR:"; break;
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
    logFile << "TIME:" << " " << QTime::currentTime().toString("hh:mm:ss") << " FILE: " << context.file << " LINE: " << QString::number(context.line) << " " << typeName << " " << mess << endl;
    cout << messFull.toStdString() << endl;
    logFile.flush();

    Qt::GlobalColor _color;
    switch(level) {
    case 0: _color = Qt::white; break;
    case 1: _color = Qt::yellow; break;
    case 2: _color = Qt::red; break;
    case QtFatalMsg: _color = Qt::red; break;
    }
    con->output(typeName + QString(' ') + mess, _color);
#if OS == 1
//    system("color 00");
#endif
    if(window) {
        switch(level) {
        case QtDebugMsg: QMessageBox::information(0, QObject::tr("Information"),
                                         mess,
                                         QMessageBox::Ok); break;
        case QtWarningMsg: QMessageBox::warning(0, QObject::tr("Warning!"),
                                     mess,
                                     QMessageBox::Ok); break;
        case QtCriticalMsg: QMessageBox::critical(0, QObject::tr("Error!"),
                                      mess,
                                      QMessageBox::Ok); break;
        case QtFatalMsg: QMessageBox::critical(0, QObject::tr("Fatal error!"),
                                               mess,
                                               QMessageBox::Ok); qApp->exit(-1);
        }
    }
}









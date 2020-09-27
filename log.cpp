#include <log.h>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDialog>
#include <QMessageBox>
#include <QApplication>
#include <iostream>
#include <global.h>
//#include <windows.h>
//#include <io.h>
//#include <fcntl.h>

QMessageBox::StandardButtons log::lastPressedButton;

//BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
//{
//   if(GetWindowThreadProcessId(hwnd, NULL) == GetCurrentThreadId())
//   {
//      *(HWND*)lParam = hwnd;
//      return FALSE;
//   }

//   return TRUE;
//}

void log::setEnabledCon(bool c, QWidget *parent) {
    if(c)
    {
        if(parent != 0)
        {
            QRect newGeometry = con->geometry();
            int newX = parent->width() + parent->geometry().x();
            int screenWidth =
                    QGuiApplication::primaryScreen()->geometry().width();
            if(newX + 600 > screenWidth)
            {
                newX = screenWidth - 600;
            }
            newGeometry.setX(newX);
            newGeometry.setY(parent->y() + 30);
            newGeometry.setWidth(600);
            newGeometry.setHeight(400);
            con->setGeometry(newGeometry);
        }
        con->show();
    }
    else con->hide();
}

console *log::init() {
    con = new console();
    con->setWindowTitle(QString("YourDroid | %1").arg(QObject::tr("Debug console")));
    return con;
}

void log::consoleSetParent(QWidget *parent)
{
    con->setParent(parent);
    con->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
}

void log::message(QtMsgType level, const QMessageLogContext &context, const QString &message) {
    bool window = false;
    QString mess = message;

    if(mess[0] == '^') {
        window = true;
        mess.remove(0, 1);
    }
    QMessageBox::StandardButtons buttons;
    if(mess[mess.length() - 1] == '|') {
        mess.chop(1);
        int pos = mess.lastIndexOf('|');
        QString buttonsString = mess.mid(pos + 1, (mess.length() - pos - 1));
        mess.chop(mess.length() - pos);
        if(buttonsString.contains('+')) buttons = QMessageBox::Ok;
        if(buttonsString.contains('-')) buttons = buttons | QMessageBox::Cancel;
        if(buttonsString.contains('y')) buttons = QMessageBox::Yes;
        if(buttonsString.contains('n')) buttons = buttons | QMessageBox::No;
    }
    else buttons = QMessageBox::Ok;

    static QString logDir;
    static bool first = true;
    if(first)
    {
        if(QFile(QCoreApplication::applicationDirPath() + "/run_as_appimage").exists())
        {
            system("echo \"Running as an appimage\"");
            logDir = "./log";
        }
        else
        {
            system("echo \"Not running as an appimage\"");
            logDir = globalGetWorkDir() + "/log";
        }
        //qDebug().noquote() << "Log directory path: " << logDir;
    }

    if(!QDir(logDir).exists()) QDir().mkdir(logDir);
    static QString logName = logDir + QString("/log-") + QDate::currentDate().toString("dMyy") + QTime::currentTime().toString("hhmmss") + ".txt";
    static QFile preLog(logName);
    if(first)
    {
        if(preLog.exists()) preLog.setFileName(logName.chopped(4) + "_.txt");
        first = false;
        QString str = QString("DEBUG: Log file path: " + preLog.fileName());
        fputs(str.toStdString().c_str(), stderr);
        system(QString("echo \"%1\"").arg(str).toStdString().c_str());
    }
    static QTextStream logFile(&preLog);
    if(!preLog.isOpen()) {
        preLog.open(QIODevice::WriteOnly);
        logFile << QString("# Created by YourDroid %1\n# Aplication dir: %2\n# Work dir: %3\n\n")
                   .arg(QString(VER_PRODUCTVERSION_STR), qApp->applicationDirPath(),
                        QDir("./").absolutePath());
        std::freopen(logName.toStdString().c_str(), "a+", stderr);
    }
    //static ofstream logFile("log.txt");
    QString typeName;
    switch(level) {
    case QtInfoMsg: typeName = "INFO:"; break;
    case QtDebugMsg: typeName = "DEBUG:"; break;
    case QtWarningMsg: typeName = "WARNING:"; break;
    case QtCriticalMsg: typeName = "ERROR:"; break;
    case QtFatalMsg: typeName = "FATAL ERROR:"; break;
    }
#if LINUX
    QString color;
    switch(level) {
    case QtWarningMsg: color = "\x1b[33m"; break;
    case QtCriticalMsg: color = "\x1b[31m"; break;
    case QtFatalMsg: color = "\x1b[31m"; break;
    default: color = "\x1b[0m"; break;
    }
    QString messFull = color + typeName + QString(' ') + mess + "\x1b[0m";
#elif WIN
    QString messFull = typeName + QString(' ') + mess;
#endif
    logFile << "TIME:" << " " << QTime::currentTime().toString("hh:mm:ss") << " FILE: " << context.file << " LINE: " << QString::number(context.line) << " " << typeName << " " << mess << "\n\r";
    std::cout << (messFull.toStdString() + '\n') << std::flush;
    logFile.flush();

    Qt::GlobalColor _color;
    switch(level) {
    case QtWarningMsg: _color = Qt::yellow; break;
    case QtCriticalMsg: _color = Qt::red; break;
    case QtFatalMsg: _color = Qt::red; break;
    default: _color = Qt::white; break;
    }
    emit con->consoleWriteMess(typeName + QString(' ') + mess, _color);

    //if(mess.isEmpty()) return;
    if(window) {
        QApplication::alert(con, 0);
        switch(level) {
        case QtWarningMsg: lastPressedButton = QMessageBox::warning(0, QObject::tr("Warning!"),
                                                                    mess,
                                                                    buttons); break;
        case QtCriticalMsg: lastPressedButton = QMessageBox::critical(0, QObject::tr("Error!"),
                                                                      mess,
                                                                      buttons); break;
        case QtFatalMsg: lastPressedButton = QMessageBox::critical(0, QObject::tr("Fatal error!"),
                                                                   mess,
                                                                   buttons); qApp->exit(-1);
        default: lastPressedButton = QMessageBox::information(0, QObject::tr("Information"),
                                                              mess,
                                                              buttons); break;
        }
    }
}









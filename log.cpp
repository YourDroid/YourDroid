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

void log::setEnabledCon(bool c) {
//    static bool open = false, succes = false;
//    static HWND hWnd;
//    if(c)
//    {
//        if(!open)
//        {
//            FreeConsole();        //на всякий случай
//            if ((succes = AllocConsole()))
//            {
//                open = true;

//                int hCrt = _open_osfhandle((long)
//                                           GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
//                *stdout = *(::_fdopen(hCrt, "w"));
//                ::setvbuf(stdout, NULL, _IONBF, 0);
//                //        *stderr = *(::_fdopen(hCrt, "w"));
//                //        ::setvbuf(stderr, NULL, _IONBF, 0);
//                EnumWindows(EnumWndProc, (LPARAM)&hWnd);
//            }
//            else return;
//        }
//        else
//        {
//            ShowWindow(hWnd, SW_SHOW);
//        }
//    }
//    else if(open)
//    {
//        ShowWindow(hWnd, SW_HIDE);
//    }
//    return;
}

console *log::init() {
    con = new console;
    con->setWindowTitle(QString("YourDroid | %1").arg(QObject::tr("Debug console")));
    return con;
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
    }
    else buttons = QMessageBox::Ok;

    if(!QDir("log").exists()) QDir().mkdir("log");
    static QString logName = qApp->applicationDirPath() + QString("/log/log-") + QDate::currentDate().toString("dMyy") + QTime::currentTime().toString("hhmmss") + ".txt";
    static QFile preLog(logName);
    static QTextStream logFile(&preLog);
    if(!preLog.isOpen()) {
        preLog.open(QIODevice::WriteOnly);
        logFile << QString("# Created by YourDroid %1\n# Work dir: %2\n\n").arg(global->VERSION, qApp->applicationDirPath());
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
    logFile << "TIME:" << " " << QTime::currentTime().toString("hh:mm:ss") << " FILE: " << context.file << " LINE: " << QString::number(context.line) << " " << typeName << " " << mess << endl;
    std::cout << (messFull.toStdString() + '\n') << std::flush;
    logFile.flush();

    Qt::GlobalColor _color;
    switch(level) {
    case QtWarningMsg: _color = Qt::yellow; break;
    case QtCriticalMsg: _color = Qt::red; break;
    case QtFatalMsg: _color = Qt::red; break;
    default: _color = Qt::white; break;
    }
    //con->output(typeName + QString(' ') + mess, _color);

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









#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QMessageLogContext>
#include <QMessageBox>
#include "console.h"

extern const QString VERSION;

class log {
    static QMessageBox::StandardButtons lastPressedButton;
    static console *con;
public:
    static QMessageBox::StandardButtons getLastPressedButton() { return lastPressedButton; }
    static console *init();
    static void consoleSetParent(QWidget *parent);
    static console *getConsole() { return con; }
    static void setEnabledCon(bool, QWidget * = 0);
    static void message(QtMsgType, const QMessageLogContext&, const QString&);
};

//QMessageBox::StandardButtons log::lastPressedButton;
//static console *con;

#endif // LOG_H








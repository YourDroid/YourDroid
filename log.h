#ifndef LOG_H
#define LOG_H

#define dFUNC_CHOOSER_3(_f1, _f2, _f3, N, ... ) N

    #define dFUNC_RECOMPOSER(argsWithParentheses)\
        dFUNC_CHOOSER_3 argsWithParentheses

    #define dMACRO_CHOOSER(target_, ...)\
        dCHOOSE_FROM_ARG_COUNT(target_, target_##_NO_ARG_EXPANDER __VA_ARGS__ ())

    #define dCHOOSE_FROM_ARG_COUNT(arg_, ...) \
        dFUNC_RECOMPOSER((__VA_ARGS__, arg_##_3, arg_##_2, arg_##_1, ))

#define dACTION_3(level, mess, mess2) log::message(level, QString(__FILE__), __LINE__, QString(mess), QString(mess2))

#define dACTION_2(level, mess) log::message(level, QString(__FILE__), __LINE__, QString(mess))

#define dACTION_NO_ARG_EXPANDER() \
        ,,,,,,,dACTION_0

    #define LOG(...)\
        dMACRO_CHOOSER( dACTION, __VA_ARGS__)(__VA_ARGS__)

//#define FULL_LOG(level, mess, mess2) log::message(level, QString(__FILE__), __LINE__, QString(mess), QString(mess2));
//#define LOG(level, mess) log::message(level, QString(__FILE__), __LINE__, QString(mess));

#include <QString>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "console.h"

extern const QString VERSION;
extern const QString &WORK_DIR;

class log {
    static console *con;
public:
    static console *init();
    static void setEnabledCon(bool c) { if(c) con->show(); else con->hide(); }
    static void messagenew(QtMsgType, const QMessageLogContext&, const QString&/*, QString = "NULL"*/);
    static void message(int level, QString file, int line, QString mes, QString = "NULL") {  }
};

static console *con;

#endif // LOG_H








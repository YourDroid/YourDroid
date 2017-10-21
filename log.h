#ifndef LOG_H
#define LOG_H

#define FULL_LOG(level, mess, mess2) log::message(level, QString(__FILE__), __LINE__, QString(mess), QString(mess2));
#define LOG(level, mess) log::message(level, QString(__FILE__), __LINE__, QString(mess));

#include <QString>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

extern const QString VERSION;

class log {
public:
    static void message(int, QString, int, QString, QString = "NULL");
};

#endif // LOG_H








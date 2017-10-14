#ifndef LOG_H
#define LOG_H

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








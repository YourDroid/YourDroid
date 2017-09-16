#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include "window.h"

class mainclass : public QObject
{
    Q_OBJECT
public:
    mainclass() {}
    int exec(int, char**);
};

#endif // MAINCLASS_H















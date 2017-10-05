#include "mainclass.h"
#include <QApplication>
#include <QString>

const QString VERSION = "0.0.1 alpha";

int main(int argc, char *argv[])
{
    mainclass one;
    return one.exec(argc,argv);
}

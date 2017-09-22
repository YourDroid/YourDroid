#include "mainclass.h"
#include <QApplication>
#include <QString>

const QString VERSION = "0.0.95 pre-alpha";

int main(int argc, char *argv[])
{
    mainclass one;
    return one.exec(argc,argv);
}

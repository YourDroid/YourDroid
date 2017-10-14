#include "mainclass.h"
#include "version.h"
#include <QApplication>
#include <QString>

const QString VERSION = VER_PRODUCTVERSION_STR;

int main(int argc, char *argv[])
{
    mainclass one;
    return one.exec(argc,argv);
}

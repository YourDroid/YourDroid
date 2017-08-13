#include "mainwindow.h"
#include "mainclass.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    mainclass one;
    return one.exec(argc,argv);
}

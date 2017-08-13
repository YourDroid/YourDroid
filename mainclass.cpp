#include <QDir>
#include <QFile>
#include <QApplication>
#include "mainwindow.h"
#include "mainclass.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

mainclass::mainclass() {
}

int mainclass::exec(int argc, char **argv) {
    QApplication app(argc,argv);
    this->setting();
    //MainWindow w;
    //w.show();
    return 0;//app.exec();
}

void mainclass::setting() {
    QFile path(pathconf);
    ofstream f("ggg.txt");
    if(path.exists()) {
        ifstream setin(pathconf, ios::binary);
        setin.read((char*)this,sizeof(mainclass));
        setin.close();
        f << 4;
    }
    else {
        QDir temp;
#if OS == 1
        temp.mkdir(string(string(getenv("APPDATA"))+string(pathset)).c_str());
#elif OS == 0
        temp.mkdir(string(string(getenv("HOME"))+pathset).c_str());
#endif
        ofstream setr(pathconf, ios::binary);
        if(setr) f << 8;
        else f << 9;
        tbios = defbios();
        arch = defarch();
#if OS == 1
        winv = defwinv();
#endif
        setr.write((char*)this,sizeof(mainclass));
        setr.close();
    }
    f << arch << tbios << winv << pathconf;
}

bool mainclass::defbios() {
#if OS == 0
    QDir boot("/boot/efi");
    return boot.exists();
#elif OS == 1
    system("mountvol a: /s");
    QDir nboot("a:\\program files");
    QDir boot("a:\\efi");
    bool ret = boot.exists() && !nboot.exists();
    //system("mountvol a: /d");
    return ret;
#endif
}

bool mainclass::defarch() {
#if OS == 0
    system("export ARCH=$(uname -m)");
    char *tarch = getenv("ARCH");
    return (*tarch=="x86") ? 0 : 1;
#elif OS == 1
    char *tarch = getenv("PROCESSOR_ARCHITECTURE"), *x = "x86";
    return (*tarch==*x) ? 0 : 1;
#endif
}

#if OS == 1
bool mainclass::defwinv() {
    QDir us("C:\\users");
    return us.exists();
}
#endif


















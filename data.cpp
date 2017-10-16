#include "data.h"
#include "log.h"
//#include <fstream>

void options::autowrite_set() {
    write_set(false,arch,tbios,winv);
}

void options::write_set(bool needSet, bool a, bool tb, bool wv) {
    log::message(0, __FILE__, __LINE__, "Writing settings...");

    if(needSet) {
        arch = a;
        tbios = tb;
#if OS == 1
        winv = wv;
#endif
    }

    QSettings settings("config.ini", QSettings::IniFormat);

    settings.beginGroup("feutures_of_pc");
    settings.setValue("archeticture", QString((a) ? "x64" : "x86"));
    settings.setValue("type_of_bios", QString((tb) ? "uefi" : "bios"));
#if OS == 1
    settings.setValue("windows_version", QString((wv) ? "vista+" : "winxp"));
#endif
    settings.endGroup();

    log::message(0, __FILE__, __LINE__, "Settings wrote succesfull");
}

bool options::read_set(bool dflt) {
    log::message(0, __FILE__, __LINE__, "Reading settings...");

    bool existConf;
    if (!dflt) existConf = QFile::exists("config.ini");
    else existConf = false;
    if(existConf) {
        log::message(0, __FILE__, __LINE__, "Settings does exist");

        QSettings settings("config.ini", QSettings::IniFormat);

        settings.beginGroup("feutures_of_pc");
        arch = (settings.value("archeticture", "x86").toString() == "x86") ? 0 : 1;
        tbios = (settings.value("type_of_bios", "uefi").toString() == "uefi") ? 1 : 0;
    #if OS == 1
        if(settings.contains("windows_version")) {
             winv = defwinv();
             //write_set(false,arch,tbios,winv);
        }
        else winv = (settings.value("windows_version", "vista+").toString() == "vista+") ? 1 : 0;
    #endif
        settings.endGroup();
    }
    else {
        log::message(0, __FILE__, __LINE__, "Settings does not exist or settings restore default");
        tbios = defbios();
        arch = defarch();
#if OS == 1
        winv = defwinv();
#endif
    }
    return existConf;

    log::message(0, __FILE__, __LINE__, "Settings read succesfull");
}

bool options::defbios() {
    log::message(0, __FILE__, __LINE__, "Defining type bios...");
#if OS == 0
    bool boot = QDir().exists("/boot/efi");
    log::message(0, __FILE__, __LINE__, (boot ? "/boot/efi exists" : "/boot/efi does not exists"));
    return boot;
#elif OS == 1
    system("mountvol a: /s");
    bool efi = QDir().exists("a:\\efi");
    log::message(0, __FILE__, __LINE__, (efi ? "a:\\efi does exists" : "a:\\efi does not exists"));
    bool bios = QDir().exists("a:\\program files");
    log::message(0, __FILE__, __LINE__, (bios ? "a:\\program files does exists" : "a:\\program files does not exists"));
    bool ret = efi && !bios;
    system("mountvol a: /d");
    return ret;
#endif
}

bool options::defarch() {
    log::message(0, __FILE__, __LINE__, "Defining architecture...");
#if OS == 0
    FILE *fp = popen("uname -m", "r");

    char buf[8];
    fgets(buf, sizeof(buf) - 1, fp);
    pclose(fp);

    QString tarch = buf;
    log::message(0, __FILE__, __LINE__, (QString("Uname returned ") + tarch));
    return (tarch=="x86\n") ? 0 : 1;
#elif OS == 1
    SYSTEM_INFO inf;
    GetNativeSystemInfo(&inf);
    log::message(0, __FILE__, __LINE__, QString("Processor type is ") + char(inf.dwProcessorType + 48));
    return inf.dwProcessorType;
#endif
}

#if OS == 1
bool options::defwinv() {
    log::message(0, __FILE__, __LINE__, "Defining windows version");
    log::message(0, __FILE__, __LINE__, (QDir().exists("c:\\users") ? "c:\\users does exists" : "c:\\users does not exists"));
    return QDir().exists("c:\\users");
}
#endif

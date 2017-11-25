#include "data.h"
#include "log.h"
#include "cmd.h"
//#include <fstream>

void options::autowrite_set() {
    write_set(false,arch,tbios,winv);
}

void options::write_set(bool needSet, bool a, bool tb, bool wv) {
    LOG(0, "Writing settings...");

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

    LOG(0, "Settings wrote succesfull");
}

bool options::read_set(bool dflt) {
    LOG(0, "Reading settings...");

    bool existConf;
    if (!dflt) existConf = QFile::exists("config.ini");
    else existConf = false;
    if(existConf) {
        LOG(0, "Settings does exist");

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
        LOG(0, "Settings does not exist or settings restoring default");
        tbios = defbios();
        arch = defarch();
#if OS == 1
        winv = defwinv();
#endif
    }
    return existConf;

    LOG(0, "Settings read succesfull");
}

bool options::defbios() {
    LOG(0, "Defining type bios...");
#if OS == 0
    bool efiExist = QDir().exists("/boot/efi");
    bool efibootmgr = !cmd("efibootmgr").exec();
    LOG(0, (efiExist ? "/boot/efi exists" : "/boot/efi does not exist"));
    LOG(0, efibootmgr ? "efibootmgr exists" : "efibootmgr does not exist");
    return efiExist || efibootmgr;
#elif OS == 1
    system("mountvol a: /s");
    bool efi = QDir().exists("a:\\efi");
    LOG(0, (efi ? "a:\\efi exists" : "a:\\efi does not exist"));
    bool bios = QDir().exists("a:\\program files");
    LOG(0, (bios ? "a:\\program files exists" : "a:\\program files does not exist"));
    bool ret = efi && !bios;
    system("mountvol a: /d");
    return ret;
#endif
}

bool options::defarch() {
    LOG(0, "Defining architecture...");
#if OS == 0
    FILE *fp = popen("uname -m", "r");

    char buf[8];
    fgets(buf, sizeof(buf) - 1, fp);
    pclose(fp);

    QString tarch = buf;
    LOG(0, (QString("Uname returned ") + tarch));
    return (tarch=="x86\n") ? 0 : 1;
#elif OS == 1
    SYSTEM_INFO inf;
    GetNativeSystemInfo(&inf);
    LOG(0, QString("Processor type is ") + char(inf.dwProcessorType + 48));
    return inf.dwProcessorType;
#endif
}

#if OS == 1
bool options::defwinv() {
    LOG(0, "Defining windows version");
    LOG(0, (QDir().exists("c:\\users") ? "c:\\users does exists" : "c:\\users does not exists"));
    return QDir().exists("c:\\users");
}
#endif

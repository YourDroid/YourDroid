#include "data.h"
#include "log.h"
#include "cmd.h"

void options::autowrite_set() {
    write_set(false, arch, tbios, winv, conEnable, lang);
}

void options::write_set(bool needSet, bool a, bool tb, bool wv, bool con, _lang l) {
    qDebug() << qApp->translate("log", "Writing settings...");

    if(needSet) {
        arch = a;
        tbios = tb;
#if OS == 1
        winv = wv;
#endif
        conEnable = con;
        lang = l;
    }

    QSettings settings("config.ini", QSettings::IniFormat);

    settings.beginGroup("settings");
    settings.setValue("enable_debug_console", con);
    settings.setValue("language", QString::fromStdString(_langHelper::to_string(lang)));
    settings.endGroup();

    settings.beginGroup("feutures_of_pc");
    settings.setValue("archeticture", QString((a) ? "x64" : "x86"));
    settings.setValue("type_of_bios", QString((tb) ? "uefi" : "bios"));
#if OS == 1
    settings.setValue("windows_version", QString((wv) ? "vista+" : "winxp"));
#endif
    settings.endGroup();

    qDebug() << qApp->translate("log", "Settings wrote succesfull");
}

bool options::read_set(bool dflt) {
    qDebug() << qApp->translate("log", "Reading settings...");

    bool existConf;
    if (!dflt) existConf = QFile::exists(qApp->applicationDirPath() + "/config.ini");
    else existConf = false;
    if(existConf) {
        qDebug() << qApp->translate("log", "Settings does exist");

        QSettings settings("config.ini", QSettings::IniFormat);

        settings.beginGroup("settings");
        conEnable = settings.value("enable_debug_console", false).toBool();
        lang = _langHelper::from_string(settings.value("language", QLocale::system().name().mid(0, 2)).toString().toStdString());
        settings.endGroup();

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
        qDebug() << qApp->translate("log", "Settings does not exist or settings restoring default");
        tbios = defbios();
        arch = defarch();
#if OS == 1
        winv = defwinv();
#endif
        conEnable = false;
        lang = _langHelper::from_string(QLocale::system().name().mid(0, 2).toStdString());
    }
    return existConf;

    qDebug() << qApp->translate("log", "Settings read succesfull");
}

bool options::defbios() {
    qDebug() << qApp->translate("log", "Defining type of bios...");
#if OS == 0
    bool efiExist = QDir().exists("/boot/efi");
    bool efibootmgr = !cmd().exec("efibootmgr").first;
    qDebug() << "/boot/efi " << qApp->translate("log", (efiExist ? "exists" : "does not exist"));
    qDebug() << "efibootmgr " << qApp->translate("log", (efibootmgr ? "exists" : "does not exist"));
    return efiExist || efibootmgr;
#elif OS == 1
    cmd::exec("mountvol a: /s");
    bool efi = QDir().exists("a:\\efi");
    qDebug() << "a:/efi " << qApp->translate("log", (efi ? "exists" : "does not exist"));
    bool bios = QDir().exists("a:\\program files");
    qDebug() << "a:/program files " << qApp->translate("log", (bios ? "exists" : "does not exist"));
    bool ret = efi && !bios;
    cmd::exec("mountvol a: /d");
    return ret;
#endif
}

bool options::defarch() {
    qDebug() << qApp->translate("log", "Defining architecture...");
#if OS == 0
    FILE *fp = popen("uname -m", "r");

    char buf[8];
    fgets(buf, sizeof(buf) - 1, fp);
    pclose(fp);

    QString tarch = buf;
    qDebug() << (qApp->translate("log", "Uname returned ") + tarch);
    return (tarch=="x86\n") ? 0 : 1;
#elif OS == 1
    SYSTEM_INFO inf;
    GetNativeSystemInfo(&inf);
    qDebug() << qApp->translate("log", "Processor type is ") + char(inf.dwProcessorType + 48);
    return inf.dwProcessorType;
#endif
}

#if OS == 1
bool options::defwinv() {
    qDebug() << "Defining windows version";
    OSVERSIONINFO osvi;
        BOOL bIsWindowsXPorLater;

        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        GetVersionEx(&osvi);
    qDebug() << QObject::tr("GetVersionEx() returns %1").arg(osvi.dwMajorVersion);
    return osvi.dwMajorVersion > 5;
}
#endif

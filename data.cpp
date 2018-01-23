#include "data.h"
#include "log.h"
#include "cmd.h"

void options::autowrite_set() {
    write_set(false, arch, tbios, winv, conEnable, lang);
}

void options::write_set(bool needSet, bool a, bool tb, bool wv, bool con, _lang l) {
    qDebug().noquote() << qApp->translate("log", "Writing settings...");

    if(needSet) {
        arch = a;
        tbios = tb;
#if WIN
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
#if WIN
    settings.setValue("windows_version", QString((wv) ? "vista+" : "winxp"));
#endif
    settings.endGroup();

    qDebug().noquote() << qApp->translate("log", "Settings wrote succesfull");
}

bool options::read_set(bool dflt) {
    qDebug().noquote() << qApp->translate("log", "Reading settings...");

    bool existConf;
    if (!dflt) existConf = QFile::exists(qApp->applicationDirPath() + "/config.ini");
    else existConf = false;
    if(existConf) {
        qDebug().noquote() << qApp->translate("log", "Settings does exist");

        QSettings settings("config.ini", QSettings::IniFormat);

        settings.beginGroup("settings");
        conEnable = settings.value("enable_debug_console", false).toBool();
        lang = _langHelper::from_string(settings.value("language", QLocale::system().name().mid(0, 2)).toString().toStdString());
        settings.endGroup();

        settings.beginGroup("feutures_of_pc");
        arch = (settings.value("archeticture", "x86").toString() == "x86") ? 0 : 1;
        tbios = (settings.value("type_of_bios", "uefi").toString() == "uefi") ? 1 : 0;
    #if WIN
        if(settings.contains("windows_version")) {
             winv = defwinv();
             //write_set(false,arch,tbios,winv);
        }
        else winv = (settings.value("windows_version", "vista+").toString() == "vista+") ? 1 : 0;
    #endif
        settings.endGroup();
    }
    else {
        qDebug().noquote() << qApp->translate("log", "Settings does not exist or settings restoring default");
        tbios = defbios();
        arch = defarch();
#if WIN
        winv = defwinv();
#endif
        conEnable = false;
        lang = _langHelper::from_string(QLocale::system().name().mid(0, 2).toStdString());
    }
    return existConf;

    qDebug().noquote() << qApp->translate("log", "Settings read succesfull");
}

bool options::defbios() {
    qDebug().noquote() << qApp->translate("log", "Defining type of bios...");
#if LINUX
    bool efiExist = QDir().exists("/boot/efi");
    bool efibootmgr = !cmd().exec("efibootmgr").first;
    qDebug().noquote() << "/boot/efi " << qApp->translate("log", (efiExist ? "exists" : "does not exist"));
    qDebug().noquote() << "efibootmgr " << qApp->translate("log", (efibootmgr ? "exists" : "does not exist"));
    return efiExist || efibootmgr;
#elif WIN
    cmd::exec("mountvol a: /s");
    bool efi = QDir().exists("a:\\efi");
    qDebug().noquote() << "a:/efi " << qApp->translate("log", (efi ? "exists" : "does not exist"));
    bool bios = QDir().exists("a:\\program files");
    qDebug().noquote() << "a:/program files " << qApp->translate("log", (bios ? "exists" : "does not exist"));
    bool ret = efi && !bios;
    cmd::exec("mountvol a: /d");
    return ret;
#endif
}

bool options::defarch() {
    qDebug().noquote() << qApp->translate("log", "Defining architecture...");
#if LINUX
    FILE *fp = popen("uname -m", "r");

    char buf[8];
    fgets(buf, sizeof(buf) - 1, fp);
    pclose(fp);

    QString tarch = buf;
    qDebug().noquote() << (qApp->translate("log", "Uname returned ") + tarch);
    return (tarch=="x86\n") ? 0 : 1;
#elif WIN
    SYSTEM_INFO inf;
    GetNativeSystemInfo(&inf);
    qDebug().noquote() << qApp->translate("log", "Processor type is ") + char(inf.dwProcessorType + 48);
    return inf.dwProcessorType;
#endif
}

#if WIN
bool options::defwinv() {
    qDebug().noquote() << "Defining windows version";
    OSVERSIONINFO osvi;

        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        GetVersionEx(&osvi);
    qDebug().noquote() << QObject::tr("GetVersionEx() returns %1").arg(osvi.dwMajorVersion);
    return osvi.dwMajorVersion > 5;
}
#endif

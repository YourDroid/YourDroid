#include "options.h"
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
    //settings.setValue("efi_part_mountpoint", efiMountPoint);
    if(!efiGuid.isEmpty()) settings.setValue("efi_part_guid", efiGuid);
#endif
    settings.endGroup();

    //qDebug().noquote() << qApp->translate("log", "Settings wrote succesfull");
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
        if(!settings.contains("windows_version")) {
            winv = defwinv();
            //write_set(false,arch,tbios,winv);
        }
        else winv = (settings.value("windows_version", "vista+").toString() == "vista+") ? 1 : 0;
        //efiMountPoint = settings.value("efi_part_mountpoint", "").toString();
        efiGuid = settings.value("efi_part_guid", "").toString();
#endif
        settings.endGroup();
    }
    else {
        qDebug().noquote() << qApp->translate("log", "Settings does not exist or settings restoring default");
        //mountEfiPart();
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

    bool ret;
    auto expr = cmd::exec("bcdedit", true);
    bool efiContain = expr.second.contains("efi");
    qDebug().noquote()
            << QObject::tr("Efi partition mounted: %1. Bcdedit output contains efi: %2. "
                           "So, type of bios is %3")
               .arg(efiMounted).arg(efiContain).arg((ret = efiContain) ? "uefi" : "bios");
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

QPair<bool, QString> options::mountEfiPart()
{
    if(efiMounted)
    {
        qDebug().noquote() << QObject::tr("The efi partition is already mounted");
        return QPair<bool, QString>(true, "");
    }

    QString mountPoint = freeMountPoint();
    auto res = cmd::exec(QString("mountvol %1 /S").arg(mountPoint));
    QString mountvolOutput = res.second;
    if(res.first == 0)
    {
        efiMounted = true;
        efiMountPoint = mountPoint;
        qDebug().noquote() << QString("The efi partition has been mounted on %1").arg(mountPoint);
        return QPair<bool, QString>(true, "");
    }

    qDebug().noquote() << "Failed to mount the efi partition. It is possibly already mounted. "
                          "Looking for it...";

    res = cmd::exec("fsutil fsinfo drives");
    QStringList mountedDrives = res.second.split(QRegExp("\\s+"));
    mountedDrives.removeAt(0);
    mountedDrives.removeAt(0);
    mountedDrives.removeLast();
    qDebug().noquote() << mountedDrives;
    QFile script("diskpart_script");
    if(script.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&script);
        stream << "list volume\n";
    }
    else
    {
        qCritical().noquote() << "Failed to create a diskpart script";
        return QPair<bool, QString>(false, "");
    }
    script.close();
    res = cmd::exec(QString("diskpart /s %1/diskpart_script").arg(qApp->applicationDirPath()));
    if(res.first)
    {
        qCritical().noquote() << "Failed to execute the diskpart script";
        return QPair<bool, QString>(false, "");
    }
    QStringList volumeList = res.second.split(QRegExp("\\s+"));

    for(auto x : mountedDrives)
    {
        qDebug().noquote() << QString("Checking %1").arg(x);
        int driveLetterIndex = volumeList.indexOf(x.chopped(2)) - 1;
        if(driveLetterIndex < 0)
        {
            qWarning().noquote() << QString("Failed to find the letter index of %1. "
                                            "Checking if the efi partition is already "
                                            "mounted with mountvol").arg(x);
            res = cmd::exec("mountvol");
            if(res.first)
            {
                qWarning().noquote() << "Can't execute mountvol";
                qWarning().noquote() << QString("Failed to check if %1 is efi partition").arg(x);
                continue;
            }
            QStringList mountvolOutput = res.second.split(QRegExp("\\s+"));
            mountvolOutput.removeLast();
            qDebug().noquote() << mountvolOutput;
            if(!mountvolOutput.at(mountvolOutput.length() - 2).contains("\\\\?\\Volume{") &&
                    mountvolOutput.last() == x)
            {
                qDebug().noquote() << QString("The efi partition is already mounted on %1").arg(x);
                efiMounted = true;
                efiMountPoint = x.chopped(1);
                efiWasMounted = true;
                return QPair<bool, QString>(true, "");
            }
            else
            {
                qDebug().noquote() << QString("%1 is not the efi partition").arg(x);
                continue;
            }
        }
        QString volumeIndex = volumeList.at(driveLetterIndex);
        qDebug().noquote() << QString("The volume index of %1 is %2").arg(x, volumeIndex);
        if(script.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&script);
            stream << "select volume %1\ndetail partition\n";
        }
        else
        {
            qCritical().noquote() << "Failed to open the diskpart script";
            return QPair<bool, QString>(false, "");
        }
        res = cmd::exec(QString("diskpart /s %1/diskpart_script").arg(qApp->applicationDirPath()));
        if(res.first)
        {
            qCritical().noquote() << "Failed to execute the diskpart script";
            return QPair<bool, QString>(false, "");
        }
        if(res.second.contains("c12a7328-f81f-11d2-ba4b-00a0c93ec93b"))
        {
            efiMounted = true;
            efiMountPoint = x.chopped(1);
            efiWasMounted = true;
            qDebug().noquote() << QString("The efi partition is already mounted on %1").arg(x);
            return QPair<bool, QString>(true, "");
        }
        qDebug().noquote() << QString("%1 is not the efi partition").arg(x);
    }
    qCritical().noquote() << "The efi partition not found";
    return QPair<bool, QString>(false, "");
}

QPair<bool, QString> options::unmountEfiPart()
{
    if(efiMounted == false)
    {
        qDebug().noquote() << QObject::tr("Efi partition is already unmounted");
    }
    qDebug().noquote() << QObject::tr("Unmounting efi partition");
    QPair<bool, QString> res;
    if(!efiWasMounted)
    {
        qDebug().noquote() << QObject::tr("Efi was not already mounted");
        auto expr = cmd::exec(QString("mountvol %1 /d").arg(efiMountPoint), true);
        if(!expr.first) qDebug().noquote() << QObject::tr("Unmounted successfully");
        else qWarning().noquote() << QObject::tr("Unmounted unsuccessfully");
        res = QPair<bool, QString>(!expr.first, expr.second);
    }
    else
    {
        qDebug().noquote() << QObject::tr("Efi was already mounted");
        res = QPair<bool, QString>(true, QString());
    }
    efiMounted = !res.first;
    return res;
}

QString options::freeMountPoint()
{
    qDebug().noquote() << QObject::tr("Looking for free drive letter");
    char i[2] = {'a', '\0'};
    for(; i[0] <= '{'; i[0]++)
    {
        if(QDir(QString(i) + ":/").exists())
        {
            qDebug().noquote() << QObject::tr("%1 exists").arg(i);
        }
        else
        {
            qDebug().noquote() << QObject::tr("%1 doesn't exist").arg(i);
            break;
        }
    }
    QString point = QString(i) + ':';
    if(point == "{:")
    {
        qDebug().noquote() << QObject::tr("No free mount points available");
        return "0";
    }
    else
    {
        qDebug().noquote() << QObject::tr("Free drive letter is %1").arg(point);
        return point;
    }
}
#endif

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
    if((ret = QDir("C:\\Windows\\Boot").exists())) qDebug().noquote() << QObject::tr("Type of bios is uefi");
    else qDebug().noquote() << QObject::tr("Type of bios is bios");
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
    auto check = [&](QString mountPoint) -> bool
    {
        auto expr = cmd::exec(QString("fsutil volume diskfree %1").arg(mountPoint));
        unsigned int size = expr.second.split(" ", QString::SkipEmptyParts).last().toInt();
        qDebug().noquote() << QObject::tr("Size is %1").arg(size);
        if(QDir(QString("%1\\efi").arg(mountPoint)).exists() && size < 524288000)
        {
            qDebug().noquote()
                    << QObject::tr("efi dir found on %1 and size is valid").arg(mountPoint);
            return true;
        }
        else
        {
            qDebug().noquote() << QObject::tr("efi dir did not find or size is unvalid");
            return false;
        }
    };

    auto mount = [&](QString guid) -> QPair<bool, QString>
    {
        efiAlreadyMounted = false;
        QString mountPoint;
        auto expr = cmd::exec(QString("wmic volume get DeviceID,DriveLetter")
                         /*.arg(partList[i].remove("\n"))*/, true);
        if(expr.first) qWarning().noquote()
                << QObject::tr("Could not get info about partition");
        QStringList tmp = expr.second.split('\n', QString::SkipEmptyParts);
        tmp.removeFirst();
        qDebug().noquote() << tmp;
        int i = tmp.indexOf(QRegExp("^" + QRegExp::escape(guid) + ".*"));
        if(i == -1)
        {
            qDebug().noquote() << QObject::tr("Could not find index");
            return QPair<bool, QString>(false, mountPoint);
        }
        QStringList partInfo = tmp[i].split(QRegExp("\\s+"), QString::SkipEmptyParts);
        qDebug().noquote() << partInfo;
        if(!expr.first && partInfo.length() == 2)
        {
            efiAlreadyMounted = true;
            mountPoint = partInfo[1];
            qDebug().noquote()
                    << QObject::tr("Partition is already mounted. So mount point is %1")
                       .arg(mountPoint);
            return QPair<bool, QString>(true, mountPoint);
        }
        else
        {
            qDebug().noquote()
                    << QObject::tr("Partition is not already mounted. "
                                   "So mount point is %1")
                       .arg(efiMountPoint);
            mountPoint = efiMountPoint;
            if(cmd::exec(QString("mountvol %1 %2")
                         .arg(efiMountPoint, guid), true).first)
            {
                qWarning().noquote() << QObject::tr("Could not mount partition");
                return QPair<bool, QString>(false, mountPoint);
            }
            else
            {
                qWarning().noquote() << QObject::tr("Partition has mounted sucessfully");
                return QPair<bool, QString>(true, mountPoint);
            }
        }
    };

    qDebug().noquote() << QObject::tr("Mounting efi partition");

    bool sucess = false;

    {
        char i[2] = {'a', '\0'};
        for(; i[0] <= '{' && QDir(QString(i) + ":/").exists(); i[0]++);
        if(i[0] == '{')
        {
            efiMountPoint = qApp->applicationDirPath() + '/'
                    + QDate::currentDate().toString("dMyy")
                    + QTime::currentTime().toString("hhmmss");
            if(!QDir().mkdir(efiMountPoint))
                efiMountPoint = qApp->applicationDirPath() + "/data/efiPartition";
        }
        else efiMountPoint = QString(i) + ":";
        qDebug().noquote() << QObject::tr("Free mountpoint is %1").arg(efiMountPoint);
    }

    if(!efiGuid.isEmpty())
    {
        qDebug().noquote() << QObject::tr("Mounting guid from settings");
        QPair<bool, QString> expr;
        if((sucess = (expr = mount(efiGuid)).first))
        {
            sucess = check(expr.second);
        }
    }
    if(!sucess)
    {
        auto expr = cmd::exec("wmic volume get DeviceID", true);
        if(expr.first)
        {
            QString err = QObject::tr("Could not get partition list: %1")
                    .arg(expr.second);
            qDebug().noquote() << err;
            return QPair<bool, QString>(false,
                                        err);
        }
        QStringList partList = expr.second.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        partList.removeFirst();
        qDebug().noquote() << partList;

        {
            qDebug().noquote() << QObject::tr("Trying by first way");
            if(!cmd::exec(QString("mountvol %1 /s").arg(efiMountPoint)).first)
            {
                qDebug().noquote()
                        << QObject::tr("Efi partition sucessfully mounted to %1").arg(efiMountPoint);
                sucess = true;
            }
            else
            {
                qDebug().noquote() << QObject::tr("Fail. Trying by second way");
                for(int i = 0; i < partList.length(); i++)
                {
//                    efiAlreadyMounted = false;
//                    QString mountPoint;
//                    expr = cmd::exec(QString("wmic volume get DeviceID,DriveLetter")
//                                     /*.arg(partList[i].remove("\n"))*/, true);
//                    if(expr.first) qWarning().noquote()
//                            << QObject::tr("Could not get info about partition");
//                    QStringList tmp = expr.second.split('\n', QString::SkipEmptyParts);
//                    tmp.removeFirst();
//                    QStringList partInfo = tmp[i].split(QRegExp("\\s+"), QString::SkipEmptyParts);
//                    qDebug().noquote() << partInfo;
//                    if(!expr.first && partInfo.length() == 2)
//                    {
//                        efiAlreadyMounted = true;
//                        mountPoint = partInfo[1];
//                        qDebug().noquote()
//                                << QObject::tr("Partition is already mounted. So mount point is %1")
//                                   .arg(mountPoint);
//                    }
//                    else
//                    {
//                        qDebug().noquote()
//                                << QObject::tr("Partition is not already mounted. "
//                                               "So mount point is %1")
//                                   .arg(efiMountPoint);
//                        mountPoint = efiMountPoint;
//                        if(cmd::exec(QString("mountvol %1 %2")
//                                     .arg(efiMountPoint, partList[i]), true).first)
//                        {
//                            qWarning().noquote() << QObject::tr("Cannot mount partition");
//                            continue;
//                        }
//                    }
                    QPair<bool, QString> mounted;
                    if(!(mounted = mount(partList[i])).first) continue;
                    QString mountPoint = mounted.second;


                    if(check(mountPoint))
                    {
                        efiMountPoint = mountPoint;
                        efiGuid = partList[i];
                        sucess = true;
                        break;
                    }
                    QPair<int, QString> expr;
                    if(!sucess && !efiAlreadyMounted) expr = cmd::exec(QString("mountvol %1 /d")
                                                                       .arg(mountPoint), true);
                    if(expr.first) break;
                }
            }
        }
    }

    {
        if(sucess)
        {
            qDebug().noquote()
                    << QObject::tr("Efi partition sucessfully mounted to %1").arg(efiMountPoint);
        }
        else qDebug().noquote() << QObject::tr("Did not find efi partition");
        return QPair<bool, QString>(sucess, "");
    }
}

QPair<bool, QString> options::unmountEfiPart()
{
    qDebug().noquote() << QObject::tr("Unmounting efi partition");
    if(!efiAlreadyMounted)
    {
        qDebug().noquote() << QObject::tr("Efi was not already mounted");
        auto expr = cmd::exec(QString("mountvol %1 /d").arg(efiMountPoint), true);
        if(!expr.first) qDebug().noquote() << QObject::tr("Unmounted sucessfully");
        else qWarning().noquote() << QObject::tr("Unmounted unsucessfully");
        return QPair<bool, QString>(!expr.first, expr.second);
    }
    else
    {
        qDebug().noquote() << QObject::tr("Efi was already mounted");
        return QPair<bool, QString>(true, QString());
    }
}
#endif

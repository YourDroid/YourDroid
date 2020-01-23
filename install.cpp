#include "install.h"
#include "log.h"
#include "window.h"
#include "cmd.h"
#include "global.h"
#include <QErrorMessage>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QPair>
#include <QStringList>

void install::addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n, bool e) {
    systems.push_back(install::_installSet(b, t, p, i, n, e));
    cntSystems = systems.length();
}

void install::write() {
    bool sysDel;
    int sysCnt = _oldSysEdit ? (cntSystems) : 0;
    //cntSystems -= deletedSys.length();

    qDebug().noquote() << qApp->translate("log", "Writing install settings...");

    QSettings install(qApp->applicationDirPath() + "/install.ini", QSettings::IniFormat);
    install.clear();
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", cntSystems);
    int finallyCnt = 0;
    for(int i = 0; i < sysCnt; i++) {
        if(systems[i].ended == false) {
            qDebug().noquote() << tr("System %1 skipped").arg(systems[i].name);
            continue;
        }
        finallyCnt = i + 1;
        qDebug().noquote() << qApp->translate("log", "System %1 config registering...").arg(systems[i].name);
        install.setValue(QString("system_") + QString::number(i), systems[i].name);
        qDebug().noquote() << qApp->translate("log", "System %1 config registered succesfull").arg(systems[i].name);
    }
    install.setValue("count_systems", finallyCnt);
    install.endGroup();

    qDebug().noquote() << QObject::tr("System configs registered succesfull");

    for(int i = 0; i < sysCnt; i++) {
        if(systems[i].ended == false) {
            qDebug().noquote() << tr("System %1 skipped").arg(systems[i].name);
            continue;
        }
        qDebug().noquote() << qApp->translate("log", "System %1 writing...").arg(QString::number(i + 1));
        QSettings system(qApp->applicationDirPath() + QString("/") +  systems[i].name + ".ini", QSettings::IniFormat);
        system.beginGroup("about");
        system.setValue("bootloader", _bootloaderHelper::to_string(systems[i].bootloader).c_str());
        system.setValue("type_place", _typePlaceHelper::to_string(systems[i].typePlace).c_str());
        system.setValue("place", systems[i].place);
        system.setValue("image", systems[i].image);
        system.setValue("os", systems[i].os);
        if(!systems[i].bcdId.isEmpty()) system.setValue("bcd_id", systems[i].bcdId);
        system.endGroup();
        qDebug().noquote() << qApp->translate("log", "System %1 wrote succesfull").arg(QString::number(i + 1));
    }
    qDebug().noquote() << qApp->translate("log", "Systems wrote succesfull");
}

void install::read() {
    qDebug().noquote() << qApp->translate("log", "Reading install settings...");

    QSettings install(qApp->applicationDirPath() + "/install.ini", QSettings::IniFormat);

    install.beginGroup("about_installing_systems");
    cntSystems = install.value("count_systems", 0).toInt();
    qDebug().noquote() << QObject::tr("Count of systems is %1").arg(cntSystems);

    for(int i = 0; i < cntSystems; i++) {
        QString name = install.value(QString("system_") + QString::number(i), i).toString();
        QString sys = name + ".ini";
        qDebug().noquote() << name;
        if(!QFile::exists(qApp->applicationDirPath() + QString("/") + sys)) {
            qCritical().noquote() << qApp->translate("log", "Config of system %1 does not exist").arg(name);
            _oldSysEdit = true;
            cntSystems = cntSystems - 1;
            continue;
        }
        QSettings system(sys, QSettings::IniFormat);
        system.beginGroup("about");
        if(!system.contains("bootloader")) {
            qCritical().noquote() << qApp->translate("log", "Config of system %1 have not section bootloader").arg(QString::number(i + 1));
        }
        else if(!system.contains("type_place")) {
            qCritical().noquote() << qApp->translate("log", "Config of system %1 does not have section type_place").arg(QString::number(i + 1));
        }
        else if(!system.contains("place")) {
            qCritical().noquote() << qApp->translate("log", "Config of system %1 does not have section place").arg(QString::number(i + 1));
        }
        if(!system.contains("os")) {
            qCritical().noquote() << QObject::tr("Config of system %1 does not have section os").arg(name);
        }
        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", "Gummiboot").toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", "Error of read!").toString().toStdString());
        QString place = system.value("place", "Error of read!").toString();
        QString image = system.value("image", "Error of read!").toString();
        bool os = system.value("os", false).toBool();
        QString _bcdId = system.value("bcd_id", "").toString();
        qDebug().noquote() << qApp->translate("log", "System ") + QString::number(i + 1) + " read succesfull";
        systems.push_back(_installSet(boot, typePlace, place, image, name, false, os, _bcdId));
        qDebug().noquote() << qApp->translate("log", "System ") + QString::number(i + 1) + " push succesfull";
        system.endGroup();
    }

    install.endGroup();

    qDebug().noquote() << "Systems read succesfull";
}

QPair<bool, QString> install::getUefiEntry(QString description)
{
    qDebug().noquote() << QObject::tr("Looking for "
                                      "the \"%1\" entry")
                          .arg(description);
    auto res = cmd::exec("bcdedit /enum firmware", true);
    if(res.first != 0) return QPair<bool, QString>(false, res.second);

    int descrBegin =
            res.second.indexOf(QString("description             ")
                               + description);
    if(descrBegin == -1)
    {
        QString error =QObject::tr("Can't find the entry "
                                  "of the description");
        qDebug().noquote() << error;
        return QPair<bool, QString>(false, error);
    }

    QString cutRight = res.second.mid(descrBegin);
    qDebug().noquote() << QObject::tr("The right part is %1")
                          .arg(cutRight);
    QString cutLeft = res.second.left(descrBegin);
    qDebug().noquote() << QObject::tr("The left part is %1")
                          .arg(cutLeft);


    int entryBegin = cutLeft.lastIndexOf("-------");
    int entryEnd = cutRight.indexOf("-------");

    QString entry;

    if(entryBegin == -1)
    {
        QString error = QObject::tr("The begin of the entry"
                                    " isn't found");
        qDebug().noquote() << error;
        return QPair<bool, QString>(false, error);
    }
    if(entryEnd == -1)
    {
        qWarning().noquote()
                << QObject::tr("Can't find the end of the entry. "
                               "The end of the bcdedit output will "
                               "be considered as the entry end");
        entry = res.second.mid(entryBegin);
    }
    else entry
            = cutLeft.mid(entryBegin)
            + cutRight.mid(1, entryEnd);
    qDebug().noquote() << QObject::tr("The entry is successfully found!"
                                      " It is %1").arg(entry);
    return QPair<bool, QString>(true, entry);
}

QPair<bool, QString> install::findUefiId(QString description, QString entry)
{
    qDebug().noquote() << QObject::tr("Looking for the id of "
                                      "the \"%1\" entry")
                          .arg(description);

    if(entry == QString())
    {
        auto expr = getUefiEntry(description);
        if(!expr.first) return expr;
        entry = expr.second;
    }

    int idBegin = entry.indexOf('{');
    int idEnd = entry.indexOf('}');
    if(idBegin == -1 || idEnd == -1)
    {
        QString error = QObject::tr("The %1 of id line"
                                    " isn't found")
                        .arg(idBegin == -1 ? "begin" : "end");
        qDebug().noquote() << error;
        return QPair<bool, QString>(false, error);
    }

    QString id = entry.mid(idBegin, idEnd - idBegin + 1);
    qDebug().noquote() << QObject::tr("The id is successfully found!"
                                      " It is %1").arg(id);

    return QPair<bool, QString>(true, id);
}

void install::registerBootloader() {
    qDebug().noquote() << tr("Registering to bootloader...");
    switch(systems.back().bootloader) {
    case _bootloader::grub2: if(global->set->tbios) registerGrub2Uefi();
                             else registerGrub2Bios();  break;
    case _bootloader::grub4dos: registerGrub4dos(); break;
        //case _bootloader::grub2_flash: qDebug().noquote() << tr("Setting up grub2 flash"); installGrub2(); break;
    }
}

void install::registerGrub4dos()
{
#if WIN
#define returnFault() return;
    qDebug().noquote() << QObject::tr("Installing grub4dos");

    QString path;
    bool res = false;

    qDebug().noquote() << QObject::tr("Copying the files");

    if(!(res = QFile((path =
                      QString("c:/yourdroid_grub4dos.mbr")))
         .exists()))
    {
        COPY(QString("%1/data/bootloaders/grub4dos"
                     "/yourdroid_grub4dos.mbr")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();

    if(!(res = QFile((path =
                      QString("c:/yg4d")))
         .exists()))
    {
        COPY(QString("%1/data/bootloaders/grub4dos"
                     "/yg4d")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();

    if(!(res = QFile((path =
                      QString("c:/ycfg.lst")))
         .exists()))
    {
        COPY(QString("%1/data/bootloaders/grub4dos"
                     "/ycfg.lst")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();


    qDebug().noquote() << QObject::tr("Setting up grub4dos");

    QString menuentry = grubLConfigure("", false, false);

    QString configFile = "c:/ycfg.lst";
    QFile _config(configFile);
    if(!_config.open(QIODevice::Append)) {
        emit abort(QObject::tr("Could not open the grub's config-file (%1)").arg(configFile));
        return;
    }
    QTextStream config(&_config);

    config << menuentry + '\n';
    _config.close();

    qDebug().noquote() << QObject::tr("adding an entry of grub4dos to UEFI");

    execAbort(QString("bcdedit /create /d \"Android\" /application bootsector"));

    QString output = resCmd.second;
    int begin = output.indexOf('{'), end = output.indexOf('}');
    QString id = output.mid(begin, end - begin + 1);
    qDebug() << QObject::tr("Id is %1").arg(id);
    systems.back().bcdId = id;

    execAbort(QString("bcdedit /set %1 device partition=C:").arg(id));

    execAbort(QString("bcdedit /set %1 path \\yourdroid_grub4dos.mbr").arg(id, systems.back().name));

    execAbort(QString("bcdedit /displayorder %1 /addlast").arg(id));

    qDebug().noquote() << QObject::tr("Grub4dos has been setted up sucessfully");

#undef returnFault
#endif
}

bool install::installGrub2Uefi() {
#if WIN
    #define returnFault() return false;

    qDebug().noquote() << "Installing Grub2 for uefi...";

#define logDirExist() qDebug().noquote() \
    << QString("%1 %2").arg(path, (res ? QObject::tr("exists") : QObject::tr("doesn't exist")));

    QString efiMountPoint = global->set->getEfiMountPoint();
    QString target = (dat->arch ? "x86_64-efi" : "i386-efi");
    QString efiFile = (dat->arch ? "grubx64.efi" : "grubia32.efi");

    QString path;
    bool res = false;

    QPair<int, QString> resGrubIns =
            cmd::exec(QString("%1/data/bootloaders/grub2/windows/grub-ins.exe "
                              "--target=%2 --efi-directory=%3 "
                              "--boot-directory=%3/yourdroid_cfg "
                              "--bootloader-id=yourdroid_grub2")
                      .arg(qApp->applicationDirPath(), target, efiMountPoint));

    if(resGrubIns.first != 0) //fault
    {
        emit abort(QString("Error while grub installation:\n %1")
                   .arg(resGrubIns.second));
        return false;
    }

    qDebug().noquote() << "Replacing the grub efi file with a fixed one";

    REMOVE(QString("%1/efi/yourdroid_grub2/%2").arg(efiMountPoint, efiFile));

    COPY(QString("%1/data/bootloaders/grub2/windows/%2").arg(qApp->applicationDirPath(), efiFile),
         (path = QString("%1/efi/yourdroid_grub2/%2").arg(efiMountPoint, efiFile)));
    logDirExist();

    qDebug().noquote() << "Making a config dir";

    if(!(res = QDir((path = QString("%1/yourdroid_cfg")
                      .arg(efiMountPoint))).exists()))
    {
        MKDIR(path);
    }
    logDirExist();

    bool grubMadeEntry = true;
    auto expr = cmd::exec("bcdedit /enum firmware", true);
    if(expr.first != 0) qDebug().noquote()
            << QObject::tr("Can't make sure if yourdroid "
                           "grub2 entry is regestered correctly");
    else
    {
        qDebug().noquote() << QObject::tr("Making sure there is an "
                                          "entry of yourdroid grub2");
        if(expr.second.contains("yourdroid_grub2"))
            qDebug().noquote()
                    << QObject::tr("There is an entry of "
                                   "yourdroid grub2");
        else
        {
            qDebug().noquote()
                    << QObject::tr("There is no entry of "
                                   "yourdroid grub2. "
                                   "So installing it");
            execAbort("bcdedit /copy {bootmgr} "
                      "/d \"yourdroid_grub2\"");
            grubMadeEntry = false;
        }

        qDebug().noquote()
                << QObject::tr("Making sure the path is stated "
                               "in the entry");
        auto resUefiEntry = getUefiEntry("yourdroid_grub2");
        auto resFindId = findUefiId("yourdroid_grub2",
                                    resUefiEntry.second);
        if(!resUefiEntry.first)
            qDebug().noquote()
                    << QObject::tr("Can't make sure if the path is stated "
                                   "in the entry because failed "
                                   "to get the entry");
        else if(!resFindId.first)
            qDebug().noquote()
                    << QObject::tr("Can't make sure if the path is stated "
                                   "in the entry because failed "
                                   "to get the id of the entry");
        else
        {
            if(!resUefiEntry.second.contains("path"))
            {
                qDebug().noquote()
                        << QObject::tr("No path stated. "
                                       "So stating it");
                execAbort(QString("bcdedit /set %1 path "
                                  "\\EFI\\yourdroid_grub2\\%2")
                          .arg(resFindId.second,
                               (dat->arch ? "grubx64.efi" :
                                            "grubia32.efi")));
            }
        }

        qDebug().noquote() << QObject::tr("Making sure the yourdroid "
                                          "grub2 entry is the first");
        int dsplOrder = expr.second.indexOf("displayorder");
        QString cut = expr.second.mid(dsplOrder);
        int fstEntryBegin = cut.indexOf('{');
        int fstEntryEnd = cut.indexOf('}');
        QString fstEntryId =
                cut.mid(fstEntryBegin,
                        fstEntryEnd - fstEntryBegin + 1);
        if(!resFindId.first)
        {
            qDebug().noquote()
                    << QObject::tr("Can't make sure if the youdroid "
                                   "grub2 entry is the first "
                                   "because failed to find its "
                                   "ID:\n%1")
                       .arg(resFindId.second);
        }
        else
        {
            if(fstEntryId == resFindId.second)
                qDebug().noquote()
                        << QObject::tr("The yourdroid grub2 entry "
                                       "is already the first");
            else
            {
                qDebug().noquote()
                        << QObject::tr("the yourdroid grub2 entry "
                                       "isn't the first, so making "
                                       "it be");
                execAbort(QString("bcdedit /set {fwbootmgr} "
                                  "displayorder %1 /addfirst")
                          .arg(resFindId.second));
            }
        }
    }

    if(!(res = QFile((path = QString("%1/yourdroid_cfg/grub.cfg")
                      .arg(efiMountPoint))).exists()))
    {
        COPY(QString("%1/data/bootloaders/grub2/windows/grub-uefi.cfg")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();

    qDebug().noquote() << QObject::tr("Grub2 has been installed successfully");

    return true;

#undef returnFault
#endif
}

void install::registerGrub2Uefi() {
#if WIN
    if(!installGrub2Uefi()) return;

    qDebug().noquote() << QObject::tr("Setting up grub");

    QString mountPoint = global->set->getEfiMountPoint();
    QString grub = QString(global->set->getArch() ? "grubx64.efi" : "grubx32.efi");

    QString menuentry = grub2Configure(QString(), false, false);

    QFile _config(mountPoint + "/yourdroid_cfg/grub.cfg");
    if(!_config.open(QIODevice::Append)) {
        emit abort(QObject::tr("Could not open the grub's config-file"));
        return;
    }
    QTextStream config(&_config);

    config << menuentry + '\n';
    _config.close();

#endif
}

bool install::installGrub2Bios() {
#if WIN
#define returnFault() return false;

    qDebug().noquote() << "Installing Grub2 for bios...";

#define logDirExist() qDebug().noquote() \
    << QString("%1 %2").arg(path, (res ? QObject::tr("exists") : QObject::tr("doesn't exist")));

    QString path;
    bool res = false;

    resCmd = cmd::exec("wmic OS GET Name /VALUE");
    if(resCmd.first != 0)
    {
        emit abort(QString("Error while grub installation:\n %1")
                   .arg(resCmd.second));
        return false;
    }

    int driveIndex = resCmd.second.indexOf("\\Partition") - 1;
    if(driveIndex < 0)
    {
        emit abort(QString("Error while grub installation:\nCannot find the drive index"));
        return false;
    }


    QPair<int, QString> resGrubIns =
            cmd::exec(QString("%1/data/bootloaders/grub2/windows/grub-ins.exe "
                              "--target=i386-pc "
                              "--boot-directory=c:/yourdroid_cfg "
                              "//./PHYSICALDRIVE%2")
                      .arg(qApp->applicationDirPath(), resCmd.second.at(driveIndex)));

    if(resGrubIns.first != 0) //fault
    {
        emit abort(QString("Error while grub installation:\n %1")
                   .arg(resGrubIns.second));
        return false;
    }

    if(!(res = QFile((path = QString("c:/yourdroid_cfg/grub/grub.cfg"))).exists()))
    {
        COPY(QString("%1/data/bootloaders/grub2/windows/grub-bios.cfg")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();

    qDebug().noquote() << QObject::tr("Grub2 has been installed successfully");

    return true;

#undef returnFault
#endif
}

void install::registerGrub2Bios() {
#if WIN
    if(!installGrub2Bios()) return;

    qDebug().noquote() << QObject::tr("Setting up grub");

    QString menuentry = grub2Configure(QString(), false, false);

    QFile _config("c:/yourdroid_cfg/grub/grub.cfg");
    if(!_config.open(QIODevice::Append)) {
        emit abort(QObject::tr("Could not open the grub's config-file"));
        return;
    }
    QTextStream config(&_config);

    config << menuentry + '\n';
    _config.close();

#endif
}

QString install::grub2Configure(QString way, bool needTimeout, bool toFile, int numSys) {
    QString name, place;
    if(numSys = -1)
    {
        place = obsolutePath(systems.back().place);
        name = systems.back().name;
    }
    else
    {
        place = obsolutePath(systems[numSys].place);
        name = systems[numSys].name;
    }
    QString menuentry = QString("menuentry '") + name + QString("' --class android-x86 {\n") +
            QString("\tsearch --file --no-floppy --set=root ") + place +  QString("/kernel\n") +
            QString("\tlinux ") + place +
            QString("/kernel root=/dev/ram0 androidboot.selinux=permissive "
                    "quiet SRC=%1\n").arg(place) +
            QString("\tinitrd ") + place + QString("/initrd.img\n") + "}\n";
    qDebug().noquote() << QObject::tr("Grub's entry is %1").arg(menuentry);

    if(toFile)
    {
        QFile _config(way);
        if(!_config.open(QIODevice::WriteOnly)) {
            emit abort(QObject::tr("Could not open the config-file"));
            return QString();
        }
        QTextStream config(&_config);
        if(needTimeout) config << "set timeout=0\n";
        config << menuentry;
        _config.close();
        return QString();
    }
    else return menuentry;
}

QString install::grubLConfigure(QString way,
                                bool needTimeout, bool toFile)
{
    QString place = obsolutePath(systems.back().place);
    QString name = systems.back().name;
    QString menuentry =
            QString("title %2\n"
                    "find --set-root %1/kernel\n"
                    "kernel %1/kernel quiet root=/dev/ram0 "
                    "androidboot.selinux=permissive "
                    "SRC=%1\n"
                    "initrd %1/initrd.img").arg(place, name);
    qDebug().noquote() << QObject::tr("Grub's entry is %1").arg(menuentry);

    if(toFile)
    {
        QFile _config(way);
        if(!_config.open(QIODevice::WriteOnly)) {
            emit abort(QObject::tr("Could not open the config-file"));
            return QString();
        }
        QTextStream config(&_config);
        if(needTimeout) config << "timeout=0\n";
        config << menuentry;
        _config.close();
        return QString();
    }
    return menuentry;
}

bool install::isInvalidImage(
        #if WIN
        QString iso
        #endif
        ) {
#if LINUX
    return (QFile(mountPoint + "/system.img").exists() || QFile(mountPoint + "/system.sfs").exists()) &&
            QFile(mountPoint + "/kernel").exists() && QFile(mountPoint + "/initrd.img").exists() &&
            QFile(mountPoint + "/ramdisk.img").exists();
#elif WIN
    QPair<int, QString> expr;
    QString command = QString("%1/data/iso-editor.exe exist %2 %3").arg(qApp->applicationDirPath(), iso);
#define checkError() \
    if(expr.first < 0 || expr.first > 2) { \
    QRegExp re("#errormesstart#\r\n(.+)\r\n#errormesend#"); \
    if (re.indexIn(expr.second) != -1) { \
    qCritical().noquote() << re.cap(1).prepend("^"); \
} \
    else qCritical().noquote() << expr.second.prepend("^"); \
    return 2; \
}
#define checkFile(file) !(bool)((expr = cmd::exec(command.arg(file))).first); checkError();
    bool systemImg = checkFile("system.img");
    bool systemSfs = checkFile("system.sfs");
    bool kernel = checkFile("kernel");
    bool initrdImg = checkFile("initrd.img");
    bool ramdiskImg = checkFile("ramdisk.img");
    sysImgOrSfs = systemSfs;
    qDebug().noquote() << sysImgOrSfs;
    return (systemImg || systemSfs) &&
            kernel && initrdImg &&
            ramdiskImg;
#endif
}

//bool install::isInvalidImage(
//        #if WIN
//        QString iso
//        #endif
//        ) {
//#if LINUX
//    return (QFile(mountPoint + "/system.img").exists() || QFile(mountPoint + "/system.sfs").exists()) &&
//            QFile(mountPoint + "/kernel").exists() && QFile(mountPoint + "/initrd.img").exists() &&
//            QFile(mountPoint + "/ramdisk.img").exists();
//#elif WIN
//    int res;
//    QPair<int, QString> expr;
//    QString command = QString("%1/data/7zip/7z.exe l %2 %3").arg(qApp->applicationDirPath(), iso);
//    auto fileExists = [&](QString file) -> bool
//    {
//        QPair<int, QString> expr = cmd::exec(command.arg(file));
//        if(expr.first) res = 2;
//        else if(expr.second.contains("0 files")) res = 1;
//        else res = 0;
//    };
//#define checkFile(file, var) \
//    bool var; \
//    expr = cmd::exec(command.arg(file)); \
//    if(expr.first) \
//    { \
//        qCritical().noquote() << expr.second.prepend("^"); \
//        var = false; \
//    } \
//    else if(expr.second.contains("0 files")) \
//    { \
//        qWarning().noquote() << QObject::tr("There is no such file: %1").arg(file); \
//        var = false; \
//    } \
//    else \
//    { \
//        qDebug().noquote() << QObject::tr("%1 is in the iso image").arg(file); \
//        var = true; \
//    }
//    checkFile("system.img", systemImg);
//    checkFile("system.sfs", systemSfs);
//    checkFile("kernel", kernel);
//    checkFile("initrd.img", initrdImg);
//    checkFile("ramdisk.img", ramdiskImg);
//    sysImgOrSfs = systemSfs;
//    return (systemImg || systemSfs) &&
//            kernel && initrdImg &&
//            ramdiskImg;
//#endif
//}

QPair<bool, QString> install::mountImage(QString image) {
    mountPoint = qApp->applicationDirPath() + QString("/iso_") + QDate::currentDate().toString("dMyy") +
            QTime::currentTime().toString("hhmmss");
    //    while(!QDir().exists(path)) {
    //        path += QString::number(rand());
    //    }
    if(!QDir().mkdir(mountPoint)) {
        return QPair<bool, QString>(false, QObject::tr("Cannot make dir for image's mount point!"));
    }
    QString command = QString("mount -o loop %1 %2").arg(image, mountPoint);
    auto expr = cmd::exec(command);
    if(expr.first) {
        return QPair<bool, QString>(false, QObject::tr("%1").arg(expr.second));
    }
    return QPair<bool, QString>(true, mountPoint);
}

void install::unmountImage() {
    QString command = QString("umount %1").arg(mountPoint);
    auto expr = cmd::exec(command);
    if(expr.first) {
        qWarning().noquote() << QObject::tr("Cannot unmount image: %1").arg(expr.second);
        return;
    }
    if((expr = cmd::exec(QString("rm -rf %1").arg(mountPoint))).first)
        qWarning().noquote() << QObject::tr("Cannot delete image's mount point: %1").arg(expr.second);
}

QString install::obsolutePath(QString path) {
#if LINUX
    auto expr = cmd::exec("grep UUID /etc/fstab | tr -s \" \" \" \"| cut -d \" \" -f 2 | sed -n '/\\/\\w/p'");
    QString fstab = expr.second, temp;
    while(fstab.count('\n') != 0) {
        temp = fstab.left(fstab.indexOf('\n') + 1);
        fstab.remove(0, fstab.indexOf('\n') + 1);
        temp = temp.remove('\n');
        qDebug().noquote() << temp;
        if(path.contains(temp)) path.remove(temp);
    }
#elif WIN
    path.remove(0, 2);
#endif
    return path;
}

void install::unpackSystem() {
    QPair<int, QString> expr;
    if(!QFile::exists(systems.back().place)) {
        qDebug().noquote() << QObject::tr("Making dir for install");
        expr = cmd::exec(QString("mkdir ") + systems.back().place);
        if(expr.first) {
            emit abort(QObject::tr("Could not make dir for install: %1").arg(expr.second));
            return;
        }
    }

    QFile copier;
    QString place = systems.back().place;
    QVector<QString> filesCopy = { "/kernel", "/ramdisk.img", "/initrd.img" };
    QString systemFile;
#if LINUX
    if(QFile(mountPoint + "/system.img").exists())
#elif WIN
    if(!sysImgOrSfs) // img
#endif
        filesCopy.push_back((systemFile = "/system.img"));
#if LINUX
    else if(QFile(mountPoint + "/system.sfs").exists())
#elif WIN
    else if(sysImgOrSfs) //sfs
#endif
        filesCopy.push_back((systemFile = "/system.sfs"));
    qDebug().noquote() << QObject::tr("System file is %1").arg(systemFile);
    qDebug().noquote() << QObject::tr("Start copying");
    bool filesExist = false;
    for(QString file : filesCopy) {

        if(file.isEmpty()) {
            emit abort(tr("File for copy is empty"));
            return;
        }
        qDebug().noquote() << QObject::tr("Copying %1").arg(file);
        if(QFile::exists(place + file)) {
            filesExist = true;
            qDebug().noquote() << QObject::tr("%1 exists. So it is going to be deleted").arg(place + file);
//#if LINUX
//            QString command = "rm -f %1";
//#elif WIN
//            QString command = "del /f /q %1";
//#endif
//            auto expr = cmd::exec(command.arg(place + file));
//            if(expr.first) {
//                emit logWindow(QtWarningMsg, QObject::tr("^Could not overwrite %1: %2").arg(place + file, expr.second));
//            }
        }
        else qDebug().noquote() << QObject::tr("%1 does not exist").arg(place + file);
        bool res = 0;
#if LINUX
        res = copier.copy(mountPoint + file, place + file);
        QString advancedInfo = "";
#elif WIN
        QPair<int, QString> expr;
        res = !(expr = cmd::exec(QString("%1/data/7zip/7z.exe x %2 %3 -o\"%4\"")
                                 .arg(qApp->applicationDirPath(), systems.back().image,
                                      file.remove(0, 1), place),
                                 false, QStringList(), filesExist ? "a\n" : "", true)).first;
        QString advancedInfo = QString(": %1").arg(expr.second);
#endif
        if(!res) {
            emit abort(QObject::tr("Could not copy %1-system file%2").arg(file, advancedInfo));
            return;
        }

        qDebug().noquote() << QObject::tr("%1 has been copied succesfully").arg(file);
//#if LINUX
//        int size = QFile(mountPoint + file).size();
//#elif WIN
//        QString command = QString("%1/data/iso-editor.exe size %2 %3")
//                .arg(qApp->applicationDirPath(), systems.back().image);
//        expr = cmd::exec(command.arg(file));
//        int size = (!expr.first) ? expr.second.toInt() : 50;
//#endif
//        emit fileEnded(size);
    }

    //    int complete = 0;
    //    QFile src(mountPoint + systemFile);
    //    QFile dst(place + systemFile);
    //    if(!src.open(QIODevice::ReadOnly)) {
    //        emit abort(QObject::tr("Could not open %1 to read").arg(mountPoint + systemFile));
    //        return;
    //    }
    //    if(!dst.open(QIODevice::WriteOnly)) {
    //        emit abort(QObject::tr("Could not open %1 to write").arg(place + systemFile));
    //        return;
    //    }
    //    char buffer[200];
    //    while(src.read(buffer, 200) != 0) {
    //         dst.write(buffer, 200);
    //         emit progressChange(200 * ++complete);
    //    }
}

void install::createDataImg(int size) {
#if LINUX
    system((QString("chmod 777 ") + qApp->applicationDirPath() + "/data/make_ext4fs/make_ext4fs").toStdString().c_str());
    QString command = qApp->applicationDirPath() + QString("/data/make_ext4fs/make_ext4fs") + QString(" -l ") + QString::number(size) +
            QString("M -a data ") + systems.back().place + QString("/data.img ");
#elif WIN
    QString command = qApp->applicationDirPath() + QString("/data/make_ext4fs/make_ext4fs.exe") + QString(" -l ") + QString::number(size) +
            QString("M -a data ") + systems.back().place + QString("/data.img ");
#endif
    cmd::exec(command);
}

void install::downloadFile(QString url, QString dest) {

    /* создаем объект для запрос

       // Выполняем запрос, получаем указатель на объект
       // ответственный за ответ
       QNetworkReply* reply=  (new QNetworkAccessManager())->get(QNetworkRequest(QUrl(url)));

    // Подписываемся на сигнал о готовности загрузки
    QObject::connect(reply, &QNetworkReply::finished, [=]() -> void {
        if (reply->error() == QNetworkReply::NoError)
        {
          QFile file(dest);
          if( file.open(QIODevice::WriteOnly) )
          {
            QByteArray content = reply->readAll();
            file.write(content); // пишем в файл
          }
        }
        else {
           QString err = reply->errorString();
           LOG(2, err, err);
        }


        reply->deleteLater();
    });*/
}

void install::delSystemFiles(int numSys) {
    progressBarDelete->setValue(0);
    qDebug().noquote() << "Deleting system files...";
    QVector<QString> files = {"/kernel", "/initrd.img", "/ramdisk.img", "/system.img", "/system.sfs", "/data.img"};
    for(auto file : files) {
        progressBarDelete->setValue(progressBarDelete->value() + 1);
        qDebug().noquote() << qApp->translate("log", "Delete ") + systems[numSys].place + file;
        statusBar->showMessage(qApp->translate("log", "Удаление ") + file);
        QFile(systems[numSys].place + file).remove();
    }
}

void install::execBars(QProgressBar *progressins, QProgressBar *progressdel, QStatusBar *status) {
    qDebug().noquote() << "Exec progress and status bars";
    progressBarInstall = progressins;
    progressBarDelete = progressdel;
    statusBar = status;
}

void install::deleteBootloaderEntry(int numSys) {
    qDebug().noquote() << "Deleting bootloader...";
    switch(systems[numSys].bootloader) {
    case _bootloader::grub2: deleteGrub2Entry(numSys); break;
    //case _bootloader::grub4dos: deleteGrubLEntry(numSys); break;
    }
}

void install::deleteGrub2Entry(int numSys) {
    qDebug().noquote() << QObject::tr("Deleting the grub2 entry of android...");
    statusBar->showMessage(QObject::tr("Deleting the grub2 entry of android"));
#if LINUX
    QFile(QString("/etc/grub.d/android/") + systems[numSys].name + ".cfg").remove();
    system("update-grub");
#elif WIN
    QFile config(QString("%1/efi/yourdroid_grub2/grub/grub.cfg")
                 .arg(global->set->efiMountPoint));
    if(!config.open(QIODevice::ReadOnly))
    {
        emit abort(QObject::tr("The config cannot be found"));
    }
    else
    {
        QTextStream configTStream(&config);
        QString configText = configTStream.readAll();
        QString grubEntry = grub2Configure(QString(), false, false, numSys);
        configText.remove(grubEntry);
        if(!config.open(QIODevice::Truncate))
        {
            //error
        }
        else configTStream << configText;
    }
#endif
    progressBarDelete->setValue(7);
}

void install::deleteSettingsEntry(int num) {
    qDebug().noquote() << "Deleting system's entry...";
    systems.remove(num);
    cntSystems--;
    qDebug().noquote() << "System's entry deleted succesfull";
    qDebug().noquote() << "Deleting system's config...";
    bool goodDel = QFile(qApp->applicationDirPath() + QString('/') + systems[num].name + ".ini").remove();
    ((goodDel) ? qDebug().noquote() : qCritical().noquote())
            << (goodDel ? QObject::tr("Entry was deleted succesfully") : QObject::tr("Entry was deleted unsuccesfully"));
}

void install::formatFlashDrive()
{
#if WIN
#define returnFault() return;
    qDebug().noquote() << QObject::tr("Formating flash drive...");
    QString part = systems.back().place;

    QPair<int, QString> addNew;
    execAbort(QString("format %1 /q /v:android /fs:ntfs").arg(part));
    qDebug().noquote() << QObject::tr("Flash drive formated successfully...");
#undef returnFault
#endif
}

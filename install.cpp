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
}

void install::write() {
    qDebug().noquote() << "Saving all systems' configs";

    settingsPath = qApp->applicationDirPath() + "/config";
    QDir settingsDir(settingsPath);
    if(!settingsDir.exists())
    {
        qDebug().noquote() << QString("%1 doesn't exist. Making it").arg(settingsPath);
        if(!settingsDir.mkdir(settingsPath))
        {
            qWarning().noquote() << "Can't make a settings dir. "
                                     "Everything is going to be saved to the application dir";
            settingsPath = qApp->applicationDirPath();
        }
    }

    for(int i = 0; i < systems.count(); i++) {
        if(systems[i].ended == false) {
            qDebug().noquote() << tr("System %1 has been skipped").arg(systems[i].name);
            continue;
        }
        qDebug().noquote() << QString("System %1 is being saved...").arg(systems[i].name);
        QSettings system(settingsPath + QString("/") +  systems[i].name + ".ini", QSettings::IniFormat);
        system.beginGroup("about");
        system.setValue("bootloader", _bootloaderHelper::to_string(systems[i].bootloader).c_str());
        system.setValue("type_place", _typePlaceHelper::to_string(systems[i].typePlace).c_str());
        system.setValue("place", systems[i].place);
        system.setValue("image", systems[i].image);
        system.setValue("os", systems[i].os);
        if(!systems[i].bcdId.isEmpty()) system.setValue("bcd_id", systems[i].bcdId);
        system.endGroup();
        qDebug().noquote() << QString("System %1 is saved succesfully")
                              .arg(systems[i].name);
    }
}

void install::read() {
    qDebug().noquote() << "Reading the installed systems' configs";

    QDir settingsDir(settingsPath);
    if(!settingsDir.exists())
    {
        qDebug().noquote() << QString("%1 doesn't exist. Looking in the application dir");
        settingsPath = qApp->applicationDirPath();
        settingsDir = QDir(settingsPath);
    }

    QStringList configs = settingsDir.entryList(QStringList() << "*.ini", QDir::Files);
    qDebug().noquote() << configs;
    qDebug().noquote() << "Excluding config.ini";
    configs.removeOne("config.ini");
    qDebug().noquote() << configs;

    for(auto x : configs)
    {
        QSettings system(settingsPath + '/' + x, QSettings::IniFormat);
        system.beginGroup("about");
        if(system.value("deleted", 0).toInt() == 1)
        {
            qDebug().noquote() << "The config is marked as deleted so it's going to be skipped";
            continue;
        }

        if(!system.contains("bootloader")) {
            qCritical().noquote() << QString("%1 doesn't have a section bootloader").arg(x);
        }
        else if(!system.contains("type_place")) {
            qCritical().noquote() << QString("%1 does not have section type_place").arg(x);
        }
        else if(!system.contains("place")) {
            qCritical().noquote() << QString("%1 does not have section place").arg(x);
        }
        if(!system.contains("os")) {
            qCritical().noquote() << QString("%1 does not have section os").arg(x);
        }

        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", "Gummiboot").toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", "Error of read!").toString().toStdString());
        QString place = system.value("place", "Error of read!").toString();
        QString image = system.value("image", "Error of read!").toString();
        bool os = system.value("os", false).toBool();
        QString _bcdId = system.value("bcd_id", "").toString();
        QString name = x;
        name.remove(".ini");
        qDebug().noquote() << QString("%1 is read succesfully").arg(x);
        systems.push_back(_installSet(boot, typePlace, place, image, name, false, os, _bcdId));
        systems.back().ended = true;
        qDebug().noquote() << QString("%1 is successfully added to the systems list").arg(x);
        system.endGroup();
    }
}

void install::sysSetSuccess()
{
    systems.back().ended = true;
}

QPair<bool, QString> install::getBcdEntry(QString description, bool efi)
{
    qDebug().noquote() << QObject::tr("Looking for "
                                      "the \"%1\" entry")
                          .arg(description);
    QPair<int, QString> res;
    if(efi) res = cmd::exec("bcdedit /enum firmware", true);
    else res = cmd::exec("bcdedit /enum", true);
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

QPair<bool, QString> install::findBcdId(QString description, QString entry)
{
    qDebug().noquote() << QObject::tr("Looking for the id of "
                                      "the \"%1\" entry")
                          .arg(description);

    if(entry == QString())
    {
        auto expr = getBcdEntry(description);
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
    bool grub2Tablet = false;
    switch(systems.back().bootloader) {
    case _bootloader::grub2_tablet: qDebug().noquote() << "Setting grub2 for tablets";
        grub2Tablet = true;
    case _bootloader::grub2:
#if LINUX
        registerGrub2Linux();
#elif WIN
        if(global->set->tbios) registerGrub2Uefi(grub2Tablet);
        else registerGrub2Bios();
#endif
        break;
    case _bootloader::win_bootloader: registerGrub4dos(); break;
        //case _bootloader::grub2_flash: qDebug().noquote() << tr("Setting up grub2 flash"); installGrub2(); break;
    }
}

bool install::installGrub4dos()
{
#if WIN
#define returnFault() return false;

    qDebug().noquote() << "Installing Grub4dos...";

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

    bool grubMadeEntry = true;
    auto expr = cmd::exec("bcdedit /enum", true);
    if(expr.first != 0) qDebug().noquote()
            << QObject::tr("Can't make sure if yourdroid "
                           "grub4dos entry is registered correctly");
    else
    {
        qDebug().noquote() << QObject::tr("Checking if there is an "
                                          "entry of yourdroid grub4dos");
        QString bcdeditOutput;
        if(expr.second.contains("Android (YourDroid)"))
            qDebug().noquote()
                    << QObject::tr("There is an entry of "
                                   "yourdroid grub4dos");
        else
        {
            qDebug().noquote()
                    << QObject::tr("There is no entry of "
                                   "yourdroid grub4dos. "
                                   "So installing it");
            execAbort(QString("bcdedit /create /d \"Android (YourDroid)\" /application bootsector"));
            grubMadeEntry = false;
            bcdeditOutput = resCmd.second;
        }

        qDebug().noquote()
                << QObject::tr("Checking if the path is stated "
                               "in the entry");
        auto resUefiEntry = getBcdEntry("Android (YourDroid)", false);
        if(bcdeditOutput == "") bcdeditOutput = resUefiEntry.second;
        auto resFindId = findBcdId("Android (YourDroid)",
                                    bcdeditOutput);
        if(!resUefiEntry.first)
            qDebug().noquote()
                    << QObject::tr("Can't make sure if the path is stated "
                                   "in the entry because failed "
                                   "to get the entry");
        if(!resFindId.first)
            qDebug().noquote()
                    << QObject::tr("Can't make sure if the path is stated "
                                   "in the entry because failed "
                                   "to get the id of the entry");
        else
        {
            systems.back().bcdId = resFindId.second;
//            if(!resUefiEntry.second.contains("partition=C:"))
//            {
                qDebug().noquote()
                        << QObject::tr("The partition is set incorrectly. "
                                       "So fixing it");
                execAbort(QString("bcdedit /set %1 device partition=C:").arg(resFindId.second));
//            }

//            if(!resUefiEntry.second.contains("path"))
//            {
                qDebug().noquote()
                        << QObject::tr("No path stated. "
                                       "So stating it");
                execAbort(QString("bcdedit /set %1 path \\yourdroid_grub4dos.mbr")
                          .arg(resFindId.second));
//            }
        }

        qDebug().noquote() << QObject::tr("Checking if the yourdroid "
                                          "grub4dos entry is the last");
//        int dsplOrder = expr.second.indexOf("displayorder");
//        QString cut = expr.second.mid(dsplOrder);
//        int fstEntryBegin = cut.indexOf('{');
//        int fstEntryEnd = cut.indexOf('}');
//        QString fstEntryId =
//                cut.mid(fstEntryBegin,
//                        fstEntryEnd - fstEntryBegin + 1);
//        if(!resFindId.first)
//        {
//            qDebug().noquote()
//                    << QObject::tr("Can't check if the yourdroid "
//                                   "grub4dos entry is the last "
//                                   "because failed to find its "
//                                   "ID:\n%1")
//                       .arg(resFindId.second);
//        }
//        else
//        {
//            if(fstEntryId == resFindId.second)
//                qDebug().noquote()
//                        << QObject::tr("The yourdroid grub4dos entry "
//                                       "is already the last");
//            else
//            {
                qDebug().noquote()
                        << QObject::tr("The yourdroid grub4dos entry "
                                       "is going to be set the last");
                execAbort(QString("bcdedit /displayorder %1 /addlast")
                          .arg(resFindId.second));
//            }
//        }
    }

    qDebug().noquote() << "Grub4dos has been installed successfully";


#undef returnFault
#endif
}

void install::registerGrub4dos()
{
#if WIN
    if(!installGrub4dos()) return;

    qDebug().noquote() << QObject::tr("Setting up grub4dos");

    QString menuentry = grubLConfigure("", false, false);

    QString configFile = "c:/ycfg.lst";
    QFile _config(configFile);
    if(!_config.open(QIODevice::Append)) {
        emit abort(QObject::tr("Could not open the grub's config-file (%1)").arg(configFile));
        return;
    }
    QTextStream config(&_config);

    config << menuentry + "\n\n";
    _config.close();

#endif
}

bool install::installGrub2Uefi(bool forTablet) {
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

    QString sourceEfiFile = efiFile;
    if(forTablet) sourceEfiFile = (dat->arch ? "grubx64_tablet.efi" : "grubia32_tablet.efi");
    COPY(QString("%1/data/bootloaders/grub2/windows/%2").arg(qApp->applicationDirPath(), sourceEfiFile),
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
        auto resUefiEntry = getBcdEntry("yourdroid_grub2");
        auto resFindId = findBcdId("yourdroid_grub2",
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
                    << QObject::tr("Can't make sure if the yourdroid "
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

void install::registerGrub2Uefi(bool forTablet) {
#if WIN
    if(!installGrub2Uefi(forTablet)) return;

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

bool install::registerGrub2Linux()
{
    qDebug().noquote() << "Registering to grub2...";
    QString tempGrub = "";
    QFile grub("/etc/grub.d/40_custom");
    bool grubExisted = true;
    if(!grub.open(QIODevice::ReadOnly))
    {
        if(!QFile().exists("/etc/grub.d/40_custom"))
        {
            qDebug().noquote() << "40_custom doesn't exist";
            grubExisted = false;
        }
        else
        {
            emit abort("Cannot open the /etc/grub.d/40_custom file");
            return false;
        }
    }

    QStringList grubStr;
    QTextStream textStream(&grub);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
            grubStr.append(line);
    }

    qDebug().noquote() << grubStr;

    QString insertString = "cat /etc/grub.d/android/*.cfg | more";
    if(grubStr.isEmpty())
    {
        tempGrub = QString("#!/bin/sh\n") + insertString;
    }
    else if(!grubStr.contains(insertString)) {
        tempGrub += grubStr[0] + QString("\n") + insertString + QString("\n");
        for(int i = 1; i < grubStr.length(); i++) tempGrub += grubStr[i] + '\n';
        qDebug().noquote() << tempGrub;
    }
    grub.close();

    if(tempGrub != "")
    {
        qDebug().noquote() << QString("The new 40_custom file:\n%1").arg(tempGrub);
        if(!grub.open(QIODevice::WriteOnly))
        {
            emit abort("Cannot open 40_custom to write in");
            return false;
        }
        textStream << tempGrub;
        grub.close();
        if(!grubExisted)
        {
            auto res = cmd::exec("chmod 777 /etc/grub.d/40_custom");
            if(res.first != 0)
            {
                emit abort(QString("Couldn't change the access permition "
                                   "of 40_custom:\n%1").arg(res.second));
                return false;
            }
            else qDebug().noquote() << "Changed the access permition of 40_custom";
        }
    }


    if(!QDir().exists("/etc/grub.d/android"))
    {
        qDebug().noquote() << "Grub android dir doesn't exist so making it";
        if(!QDir().mkdir("/etc/grub.d/android"))
        {
            emit abort("Cannot make an android grub directory");
            return false;
        }
    }

    grub2Configure(QString("/etc/grub.d/android/") + systems.back().name + ".cfg");

    auto res = cmd::exec("update-grub");
    if(res.first == 0)
    {
        qDebug().noquote() << "Grub2 has been installed";
        return true;
    }
    else
    {
        emit abort(QString("Error while grub installation:\n%1").arg(res.second));
        return false;
    }
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
    sysTypeSfs = QFile(mountPoint + "/system.sfs").exists();
    return (QFile(mountPoint + "/system.img").exists() || QFile(mountPoint + "/system.sfs").exists()) &&
            QFile(mountPoint + "/kernel").exists() && QFile(mountPoint + "/initrd.img").exists() &&
            QFile(mountPoint + "/ramdisk.img").exists();
#elif WIN
    auto res = cmd::exec(QString("%1/data/7zip/7z.exe l %2")
                         .arg(qApp->applicationDirPath(), iso));
    if(res.first != 0)
    {
        qCritical().noquote() << "Can't execute 7zip";
        return false;
    }

    bool systemImg = res.second.contains("system.img");
    bool systemSfs = res.second.contains("system.sfs");
    bool kernel = res.second.contains("kernel");
    bool initrdImg = res.second.contains("initrd.img");
    bool ramdiskImg = res.second.contains("ramdisk.img");
    qDebug().noquote() << QString("system.img: %1; system.sfs: %2; kernel: %3; "
                                  "initrd.img: %4; ramdisk.img: %5")
                          .arg(QString::number(systemImg), QString::number(systemSfs),
                               QString::number(kernel), QString::number(initrdImg),
                               QString::number(ramdiskImg));
    sysTypeSfs = systemSfs;
    qDebug().noquote() << QString("sysTypeSfs = %1").arg(QString::number(sysTypeSfs));
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
//    sysTypeSfs = systemSfs;
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
    auto expr = cmd::exec(QString("sh %1/data/fstab.sh")
                          .arg(qApp->applicationDirPath()));
    QStringList fstab = expr.second.split(QRegExp("\\s+"));
    qDebug().noquote() << fstab;
    for(auto x : fstab)
    {
        if(path.contains(x)) path.remove(x);
    }
    qDebug().noquote() << QString("The path is %1").arg(path);
#elif WIN
    path.remove(0, 2);
#endif
    return path;
}

void install::formatPartExt4()
{
    qDebug().noquote() << QString("%1 is going to be formated to ext4").arg(systems.back().place);
    auto res = cmd::exec(QString("%1/data/ext2fsd/mke2fs.exe -t ext4 %2")
                         .arg(qApp->applicationDirPath(), systems.back().place),
                         false, QStringList(), "\n");
    if(res.first != 0)
    {
        emit abort(QObject::tr("Cannot format the selected partition to ext4: %1")
                   .arg(res.second));
        return;
    }

}

void install::unpackSystem(sysImgExtractType sysType) {
    QPair<int, QString> expr;
    if(!QFile::exists(systems.back().place)) {
        if(systems.back().place.length() <= 3)
        {
            emit abort(QObject::tr("Can't access the selected drive. Please make sure you have "
                                   "ext4fsd installed on your computer"));
            return;
        }
        else
        {
            qDebug().noquote() << QObject::tr("Making dir for install");
            expr = cmd::exec(QString("mkdir ") + systems.back().place);
            if(expr.first) {
                emit abort(QObject::tr("Could not make dir for install: %1").arg(expr.second));
                return;
            }
        }
    }

    QFile copier;
    QString place = systems.back().place;
    if(place.back() == '/' || place.back() == '\\') place.chop(1);

    QVector<QString> filesCopy = { "/kernel", "/ramdisk.img", "/initrd.img" };
    QString systemFile;
#if LINUX
    if(QFile(mountPoint + "/system.img").exists())
#elif WIN
    if(!sysTypeSfs) // img
#endif
        filesCopy.push_back((systemFile = "/system.img"));
#if LINUX
    else if(QFile(mountPoint + "/system.sfs").exists())
#elif WIN
    else if(sysTypeSfs) //sfs
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

        qDebug().noquote() << QObject::tr("%1 has been copied").arg(file);
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

    qDebug().noquote() << "Checking if the files exist";

    for(QString file : filesCopy) {
        qDebug().noquote() << QString("Checking %1").arg(place + file);
        if(!QFile::exists(place + file)) {
            emit abort(QObject::tr("Could not copy %1-system file: unknown reason").arg(file));
            return;
        }
        else qDebug().noquote() << QString("%1 exists").arg(file);
    }

    if(sysType == sysImgExtractType::toImg || sysType == sysImgExtractType::toFolder)
    {
        qDebug().noquote() << "Extracting system.img out of system.sfs";
        if(sysTypeSfs) //sfs
        {
            bool res = false;
#if WIN
            QPair<int, QString> expr;
            res = !(expr = cmd::exec(QString("%1/data/7zip/7z.exe x %2 %3 -o\"%4\"")
                                     .arg(qApp->applicationDirPath(), place + "/system.sfs",
                                          "system.img", place),
                                     false, QStringList(), filesExist ? "a\n" : "", true)).first;
            QString advancedInfo = QString(": %1").arg(expr.second);
#elif LINUX
            QPair<int, QString> expr;
            expr = cmd::exec(QString("chmod +x %1/data/p7zip/7z")
                             .arg(qApp->applicationDirPath()));
            if(expr.first != 0) qWarning().noquote() << "Cannot do chmod +x 7z";

            res = !(expr = cmd::exec(QString("%1/data/p7zip/7z x %2 %3 -o\"%4\"")
                                     .arg(qApp->applicationDirPath(), place + "/system.sfs",
                                          "system.img", place),
                                     false, QStringList(), filesExist ? "a\n" : "", true)).first;
            QString advancedInfo = QString(": %1").arg(expr.second);
#endif
            if(res && QFile::exists(place + "/system.img"))
            {
                qDebug().noquote() << "Success. Deleting system.sfs";
                if(!QFile::remove(place + "/system.sfs")) qWarning().noquote()
                        << QObject::tr("^Cannot delete system.sfs");
            }
            else
            {
                qWarning().noquote() << QObject::tr("^The system is going to be set as read-only"
                                                     " because of the failure in extracting "
                                                     "system.img") + advancedInfo;
                return;
            }
        }
        else qDebug().noquote() << "The system file is already .img";
        if(sysType == sysImgExtractType::toFolder)
        {
            qDebug().noquote() << "extracting system.img to /system";
#if LINUX
            auto res = cmd::exec(QString("%1/data/p7zip/7z x %2 -o\"%4\"")
                                 .arg(qApp->applicationDirPath(), place + "/system.img",
                                      place + "/system"));
#elif WIN
            auto res = cmd::exec(QString("%1/data/7zip/7z.exe x %2 -o\"%4\"")
                                 .arg(qApp->applicationDirPath(), place + "/system.img",
                                      place + "/system"));
#endif
            if(res.first == 0 && QDir(place + "/system").exists())
            {
                qDebug().noquote() << "Success. Deleting the system.img";
                if(!QFile::remove(place + "/system.img")) qWarning().noquote()
                        << QObject::tr("^Cannot delete system.img");
            }
            else
            {
                qWarning().noquote() << QObject::tr("^The system is extracted as .img "
                                                    "because of the failure in "
                                                    "extracting system.img: %1")
                                        .arg(res.second);
                return;
            }
        }
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

void install::createDataImg(int size, bool toFolder) {
    if(toFolder)
    {
        if(!QDir().mkdir(systems.back().place + "/data"))
        {
            emit abort(QObject::tr("Cannot make a directory for /data.\n"
                                   "Try creating an .img file instead"));
            return;
        }
    }
    else
    {
#if LINUX
        system((QString("chmod 777 ") + qApp->applicationDirPath() + "/data/make_ext4fs/make_ext4fs").toStdString().c_str());
        QString command = qApp->applicationDirPath() + QString("/data/make_ext4fs/make_ext4fs") + QString(" -l ") + QString::number(size) +
                QString("M -a data ") + systems.back().place + QString("/data.img ");
#elif WIN
        QString command = qApp->applicationDirPath() + QString("/data/make_ext4fs/make_ext4fs.exe") + QString(" -l ") + QString::number(size) +
                QString("M -a data ") + systems.back().place + QString("/data.img ");
#endif
        auto res = cmd::exec(command);
        if(res.first != 0)
        {
            emit abort(QObject::tr("Cannot create an image for /data: %1")
                       .arg(res.second));
            return;
        }
    }
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
    qDebug().noquote() << "Deleting android files...";

    if(systems[numSys].place.count() > 3)
    {
        statusBar->showMessage(QObject::tr("log", "Deleting android files"));
        if(!QDir(systems[numSys].place).removeRecursively())
        {
            qCritical().noquote() << QObject::tr("^Cannot delete android files");
            return;
        }
        qDebug().noquote() << "Deleting the folder itself";
        if(!QDir().remove(systems[numSys].place))
        {
            qDebug().noquote() << "Cannot delete the folder";
        }
    }
    else
    {
        QVector<QString> files = {"/kernel", "/initrd.img", "/ramdisk.img", "/system.img",
                                  "/system.sfs", "/data.img"};
        for(auto file : files) {
            progressBarDelete->setValue(progressBarDelete->value() + 1);
            qDebug().noquote() << QObject::tr("log", "Deleting ") + systems[numSys].place + file;
            statusBar->showMessage(QObject::tr("log", "Deleting ") + file);
            QFile(systems[numSys].place + file).remove();
        }
    }

    QDir sysDir = systems[numSys].place + "/system";
    if(sysDir.exists())
    {
        qDebug().noquote() << "Deleting /system folder";
        sysDir.removeRecursively();
        sysDir.remove(systems[numSys].place + "/system");
    }

    QDir dataDir = systems[numSys].place + "/data";
    if(sysDir.exists())
    {
        qDebug().noquote() << "Deleting /data folder";
        dataDir.removeRecursively();
        dataDir.remove(systems[numSys].place + "/data");
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
    case _bootloader::grub2_tablet:
    case _bootloader::grub2: deleteGrub2Entry(numSys); break;
    case _bootloader::win_bootloader:
    case _bootloader::grub4dos: deleteGrubLEntry(numSys); break;
    }
}

void install::deleteGrubLEntry(int numSys)
{
#if WIN
    qDebug().noquote() << QObject::tr("Deleting the grub legacy entry of android...");
    statusBar->showMessage(QObject::tr("Deleting the grub legacy entry of android"));

    QString cfgPath = "c:/ycfg.lst";
    QFile config(cfgPath);
    if(!config.open(QIODevice::ReadOnly))
    {
        emit abort(QObject::tr("The config cannot be found"));
        return;
    }
    else
    {
        QTextStream configTStream(&config);
        QString configText = configTStream.readAll();
        int entryBeginIndex = configText.indexOf(QString("title %1").arg(systems[numSys].name));
        if(entryBeginIndex == -1)
        {
            emit abort(QObject::tr("Cannot find the begining of the entry"));
            return;
        }

        int entryEndIndex = configText.indexOf("initrd.img", entryBeginIndex);
        if(entryEndIndex == -1)
        {
            emit abort(QObject::tr("Cannot find the end of the entry"));
            return;
        }
        entryEndIndex += 9; //index of the last letter

        QString entry = configText.mid(entryBeginIndex, entryEndIndex - entryBeginIndex + 3);

        qDebug().noquote() << entry;

        configText.remove(entry);
        config.close();
        if(!config.open(QIODevice::WriteOnly))
        {
            emit abort(QObject::tr("Cannot open the grub legacy config as write only"));
            return;
        }
        else configTStream << configText;
    }

    progressBarDelete->setValue(7);
#endif
}

void install::deleteGrub2Entry(int numSys) {
    qDebug().noquote() << QObject::tr("Deleting the grub2 entry of android...");
    statusBar->showMessage(QObject::tr("Deleting the grub2 entry of android"));
#if LINUX
    if(QFile(QString("/etc/grub.d/android/") + systems[numSys].name + ".cfg").remove())
    {
        emit abort(QObject::tr("Cannot delete the entry in grub2"));
        return;
    }
    if(cmd::exec("update-grub").first != 0)
    {
        qWarning().noquote() << QObject::tr("^Cannot update grub");
    }
#elif WIN
    QString cfgPath;
    if(global->set->tbios) cfgPath = QString("%1/yourdroid_cfg/grub.cfg")
                                           .arg(global->set->efiMountPoint);
    else cfgPath = "c:/yourdroid_cfg/grub/grub.cfg";
    qDebug().noquote() << QString("The config path is %1").arg(cfgPath);
    QFile config(cfgPath);
    if(!config.open(QIODevice::ReadOnly))
    {
        emit abort(QObject::tr("The config cannot be found"));
        return;
    }
    else
    {
        QTextStream configTStream(&config);
        QString configText = configTStream.readAll();
        int entryBeginIndex = configText.indexOf(QString("menuentry '%1'").arg(systems[numSys].name));
        if(entryBeginIndex == -1)
        {
            emit abort(QObject::tr("Cannot find the begining of the entry"));
            return;
        }

        int entryEndIndex = configText.indexOf('}', entryBeginIndex);
        if(entryEndIndex == -1)
        {
            emit abort(QObject::tr("Cannot find the end of the entry"));
            return;
        }

        QString entry = configText.mid(entryBeginIndex, entryEndIndex - entryBeginIndex + 3);

        qDebug().noquote() << entry;

        configText.remove(entry);
        config.close();
        if(!config.open(QIODevice::WriteOnly))
        {
            emit abort(QObject::tr("Cannot open the grub2 config as write only"));
            return;
        }
        else configTStream << configText;
    }
#endif
    progressBarDelete->setValue(7);
}

void install::deleteSettingsEntry(int num) {
    qDebug().noquote() << "Deleting the system's entry...";
    systems.remove(num);

    qDebug().noquote() << "Marking the entry as deleted";
    QString config = settingsPath + '/' + systems[num].name + ".ini";
    qDebug().noquote() << config;
    QSettings system(config, QSettings::IniFormat);
    system.beginGroup("about");
    system.setValue("deleted", 1);

    qDebug().noquote() << "Deleting the system's config...";
    if(!QFile::remove(config))
    {
        qWarning().noquote() << "The config cannot be deleted";
    }
    else
    {
        qDebug().noquote() << "The config has been deleted";
    }
#if WIN
        cmd::exec(QString("del %1").arg(config));
#elif LINUX
        cmd::exec(QString("rm -rf %1").arg(config));
#endif
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

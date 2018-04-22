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
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", cntSystems);
    int finallyCnt = 0;
    for(int i = 0; i < sysCnt; i++) {
        if(systems[i].ended == false) {
            qDebug().noquote() << tr("System %1 skipped").arg(systems[i].name);
            continue;
        }
        finallyCnt = i;
        qDebug().noquote() << qApp->translate("log", "System %1 config registering...").arg(systems[i].name);
        install.setValue(QString("system_") + QString::number(i), systems[i].name);
        qDebug().noquote() << qApp->translate("log", "System %1 config registered succesfull").arg(systems[i].name);
    }
    install.setValue("count_systems", finallyCnt);
    install.endGroup();

    qDebug().noquote() << qApp->translate("log", "System configs registered succesfull");

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

void install::registerBootloader() {
    qDebug().noquote() << tr("Registering to bootloader...");
    switch(systems.back().bootloader) {
    case _bootloader::grub2: qDebug().noquote() << tr("Registering to grub2"); registerGrub2(); break;
    case _bootloader::gummiboot: qDebug().noquote() << tr("Registering to gummiboot"); registerGummi(); break;
    case _bootloader::bootmgr: qDebug().noquote() << tr("Registering to bootmgr"); registerBootmgr();
        //case _bootloader::grub2_flash: qDebug().noquote() << tr("Setting up grub2 flash"); installGrub2(); break;
    }
}

void install::installGrub2(install2Flash _install2Flash) {
#if WIN
    QString efiDrive;
    if(_install2Flash) efiDrive = systems.back().place;
    else efiDrive = global->set->getEfiMountPoint();
    grub2Configure(qApp->applicationDirPath() + "/tempGrubConf");
#endif

}

void install::registerBootmgr()
{
#if WIN
    qDebug().noquote() << QObject::tr("Setting up bootmgr");

    QString path;
    bool res = false;
    QString grubPath = QString("c:/yourdroid_boot/%1").arg(systems.back().name);

    if(!(res = QDir((path = "c:/yourdroid_boot")).exists()))
    {
        MKDIR(path);
    }
    logDirExist();

    res = QDir((path = grubPath)).exists();
    logDirExist();
    if(res)
    {
        qDebug().noquote()
                << QObject::tr("^Grub folder with a same name exists. "
                               "Do you want to overwrite it "
                               "(if no, installation will abort)?|+-");
        if(log::getLastPressedButton() == QMessageBox::Ok)
        {
            if(!QDir(path).removeRecursively())
            {
                emit abort(QObject::tr("Couldn't overwrite"));
                return;
            }
        }
        else
        {
            emit abort(QObject::tr("Canceled by user"));
            return;
        }
    }
    else MKDIR(path);

    qDebug().noquote() << QObject::tr("Selecting grub4dos file name...");

    srand((unsigned int)time(NULL));
    QString grName;
    while(1)
    {
        char *ch = new char[3] {rand() % 256 - 128, '\0'};
        grName = QString("gr_%1").arg(ch);
        bool exist = QFile(QString("C:/%1").arg(grName)).exists();
        if(exist) qDebug().noquote() << QObject::tr("%1 exists").arg(grName);
        else qDebug().noquote() << QObject::tr("%2 doesn't exist").arg(grName);
    }

    qDebug().noquote() << QObject::tr("Grub4dos file name is %1").arg(grName);

    COPY(QString("%1/data/bootloaders/grub2/simple").arg(qApp->applicationDirPath()),
         QString("C:/%1").arg(grName));

    //DEBUG("Editing grub4dosfile");
    QFile _gr(QString("c:/%1").arg(grName));
    if(!_gr.open(QIODevice::Append | QIODevice::WriteOnly))
    {
        emit abort(QObject::tr("Couldn't open the grub file"));
        return;
    }
    QTextStream gr(&_gr);
    gr << QString("yourdroid_boot/%1/menu.lst");
    _gr.close();


    QString grMbrPath = QString("%1/grubLegasy.mbr").arg(grubPath);

    COPY(QString("%1/data/bootloaders/grub_legasy/grldr.mbr").arg(qApp->applicationDirPath()),
         grMbrPath);

    //DEBUG("Editing grub4dos mbr file...");
    QFile _grMbr(grMbrPath);
    if(!_grMbr.open(QIODevice::ReadOnly))
    {
        emit abort(QObject::tr("Couldn't open the grub.mbr to read"));
        return;
    }
    QTextStream grMbr(&_grMbr);
    QString grMbrText = grMbr.readAll();
    grMbrText.replace("smpl", grName);
    if(!_grMbr.open(QIODevice::WriteOnly))
    {
        emit abort(QObject::tr("Couldn't open the grub.mbr to write"));
        return;
    }
    grMbr << grMbrText;

    grubLConfigure(qApp->applicationDirPath() + "/tempGrubConf", true);

    COPY(QString("%1/tempGrubConf").arg(qApp->applicationDirPath()),
         QString("%1/menu.lst").arg(grubPath));

    execAbort(QString("bcdedit /create /d \"%1\" /application bootsector").arg(systems.back().name));

    QString output = resCmd.second;
    int begin = output.indexOf('{'), end = output.indexOf('}');
    QString id = output.mid(begin, end - begin + 1);
    qDebug() << QObject::tr("Id is %1").arg(id);
    systems.back().bcdId = id;

    execAbort(QString("bcdedit /set %1 device partition=C:").arg(id));

    execAbort(QString("bcdedit /set %1 path \\yourdroid_boot\\%2\\grubLegasy.mbr").arg(id, systems.back().name));

    execAbort(QString("bcdedit /displayorder %1 /addlast").arg(id));

    qDebug().noquote() << QObject::tr("Gummiboot setted up sucessfully");
#endif
}

void install::registerGummi() {
#if WIN
    installGummi(isRegUefiGummi());

    qDebug().noquote() << QObject::tr("Setting up gummiboot");

    QString mountPoint = global->set->getEfiMountPoint();
    QString grub = QString(global->set->getArch() ? "grub64.efi" : "grub32.efi");

    grub2Configure(qApp->applicationDirPath() + "/tempGrubConf");

    QFile _config(qApp->applicationDirPath() + "/tempGummiConf");
    if(!_config.open(QIODevice::WriteOnly)) {
        emit abort(QObject::tr("Could not open the gummiboot's config-file"));
        return;
    }
    QTextStream config(&_config);
    config << QString("title %1\nefi %2\\%3")
              .arg(systems.back().name,
                   QString("%1/efi/yourdroid_gummiboot/%2")
                   .arg(mountPoint, systems.back().name).replace('/', "\\"), grub);
    _config.close();

    QString path;
    bool res = false;

    if(!(res = QDir((path = QString("%1/efi/yourdroid_gummiboot/%2").arg(mountPoint, systems.back().name)))
         .exists()))
    {
        MKDIR(path);
        COPY(QString("%1/data/bootloaders/gummi/loader/entries/0windows.conf")
             .arg(qApp->applicationDirPath()), path + "/0windows.conf");
        COPY(QString("%1/tempGrubConf").arg(qApp->applicationDirPath()), path + "/grub.cfg");
        COPY(QString("%1/data/bootloaders/grub2/%2").arg(qApp->applicationDirPath(), grub),
             path + '/' + grub);
        //        execAbort(QString("copy %1/data/bootloaders/gummi/loader/entries/0windows.conf+"
        //                          "%1/tempGrubConf+"
        //                          "%1/data/bootloaders/grub2/%2 %3")
        //                  .arg(qApp->applicationDirPath(),
        //                       grub, path));
    }
    logDirExist();

    res = QFile((path = QString("%1/loader/entries/%2.conf").arg(mountPoint, systems.back().name)))
            .exists();
    logDirExist();
    if(res)
    {
        qDebug().noquote()
                << QObject::tr("^Do you want to overwrite system's config "
                               "(if no, installation will abort)?|+-");
        if(log::getLastPressedButton() == QMessageBox::Ok)
        {
            RENAME(path, path + "_old");
        }
        else emit abort(QObject::tr("canceled by user"));
    }
    COPY(QString("%1/tempGummiConf").arg(qApp->applicationDirPath()),
         path);
    //    execAbortSep(QString("copy %1/tempGummiConf %2")
    //              .arg(qApp->applicationDirPath(), path));
#endif

    qDebug().noquote() << QObject::tr("Gummiboot setted up sucessfully");
}

bool install::isRegUefiGummi() {
    qDebug().noquote() << "Checking gummiboot's entry";
#if WIN
    auto expr = cmd::exec(QString("bcdedit /enum firmware"), true);
    QFile _enum(QString("%1/tempEnum").arg(qApp->applicationDirPath()));
    if(!_enum.open(QIODevice::WriteOnly)) {
        qWarning().noquote() << QObject::tr("Could not open tempEnum");
        return false;
    }
    QTextStream(&_enum) << expr.second;
    _enum.close();
    bool res = !cmd::exec(
                QString("find \"YourDroid Gummiboot\" %1/tempEnum").arg(qApp->applicationDirPath())).first;
    qDebug().noquote() << (res ? "Gummiboot registered" : "Gummiboot did not registered");
    return res;
#endif
}

void install::installGummi(bool isRegUefi) {
    qDebug().noquote() << "Installing gummiboot...";
#if WIN

    if(!isRegUefi)
    {
        execAbort("bcdedit /copy {bootmgr} /d \"YourDroid Gummiboot\"");

        QString output = resCmd.second;
        int begin = output.indexOf('{'), end = output.indexOf('}');
        QString id = output.mid(begin, end - begin + 1);
        qDebug() << QObject::tr("Id is %1").arg(id);

        execAbort(QString("bcdedit /set %1 path \\EFI\\yourdroid_gummiboot/%2")
                  .arg(id, (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi")));

        execAbort(QString("bcdedit /set {fwbootmgr} displayorder %1 /addfirst").arg(id));
    }

#define logDirExist() qDebug().noquote() \
    << QString("%1 %2").arg(path, (res ? QObject::tr("exists") : QObject::tr("doesn't exist")));

    QString s = global->set->getEfiMountPoint();

    QString path;
    bool res = false;
    if(!(res = QDir((path = QString("%1/EFI/yourdroid_gummiboot").arg(s))).exists()))
    {
        MKDIR(path);
    }
    logDirExist();

    QString gummiFile = (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi");

    if(!(res = QFile((path = QString("%1/EFI/yourdroid_gummiboot/%2")
                      .arg(s, gummiFile))).exists()))
    {
        COPY(qApp->applicationDirPath() + "/data/bootloaders/gummi/"
             + gummiFile,
             path);
    }
    logDirExist();

    if(!(res = QDir((path = QString("%1/loader").arg(s))).exists()))
    {
        MKDIR(path);
    }
    logDirExist();

    if(!(res = QFile((path = QString("%1/loader/loader.conf").arg(s))).exists()))
    {
        COPY(QString("%1/data/bootloaders/gummi/loader/loader.conf")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();

    if(!(res = QDir((path = QString("%1/loader/entries").arg(s))).exists()))
    {
        MKDIR(path);
    }
    logDirExist();

    if(!(res = QFile((path = QString("%1/loader/entries/0windows.conf").arg(s))).exists()))
    {
        COPY(QString("%1/data/bootloaders/gummi/loader/entries/0windows.conf")
             .arg(qApp->applicationDirPath()),
             path);
    }
    logDirExist();


#endif

    qDebug().noquote() << QObject::tr("Gummiboot installed succesful");
}

void install::registerGrub2() {
    qDebug().noquote() << QObject::tr("Registering to grub2...");
    QFile _grub("/etc/grub.d/40_custom");
    if(!_grub.open(QIODevice::ReadOnly)) {
        emit abort(QObject::tr("Could not open the grub config-file to read"));
        return;
    }
    QTextStream grub(&_grub);
    QVector<QString> grubStr;
    while(!grub.atEnd()) {
        grubStr.push_back(grub.readLine());
    }
    _grub.close();
    if(!_grub.open(QIODevice::WriteOnly)) {
        emit abort(QObject::tr("Could not open the grub config-file to write"));
        return;
    }
    qDebug().noquote() << QObject::tr("Grub-config:");
    for(QString str : grubStr) qDebug().noquote() << str;
    QPair<int, QString> expr;
    if(grubStr[1] != "cat /etc/grub.d/android/*.cfg | more") {
        grub << grubStr[0] << "\ncat /etc/grub.d/android/*.cfg | more\n";
        for(int i = 1; i < grubStr.length(); i++) grub << grubStr[i] << '\n';
        if(!QDir().mkdir("/etc/grub.d/android"))
            if((expr = cmd::exec("mkdir /etc/grub.d/android")).first) {
                emit abort(QObject::tr("Could not make dir for configs: %1")
                           .arg(expr.second));
                return;
            }
    }
    grub2Configure(QString("/etc/grub.d/android/") + systems.back().name + ".cfg");
    qDebug().noquote() << QObject::tr("Updating grub...");
    if((expr = cmd::exec("update-grub")).first) {
        emit abort(QObject::tr("Could not update grub: %1").arg(expr.second));
        return;
    }
}

void install::grub2Configure(QString way, bool needTimeout) {
    QString place = obsolutePath(systems.back().place);
    QString name = systems.back().name;
    QFile _config(way);
    if(!_config.open(QIODevice::WriteOnly)) {
        emit abort(QObject::tr("Could not open the config-file"));
        return;
    }
    QTextStream config(&_config);
    if(needTimeout) config << "set timeout=0\n";
    config << (QString("menuentry '") + name + QString("' --class android-x86 {\n") +
               QString("\tsearch --file --no-floppy --set=root ") + place +  QString("/kernel\n") +
               QString("\tlinux ") + place +
               QString("/kernel root=/dev/ram0 androidboot.hardware=android-x86 "
                       "androidboot.selinux=permissive SRC=%1\n").arg(place) +
               QString("\tinitrd ") + place + QString("/initrd.img\n") + "}\n");
    _config.close();
}

void install::grubLConfigure(QString way, bool needTimeout)
{
    QString place = obsolutePath(systems.back().place);
    QString name = systems.back().name;
    QFile _config(way);
    if(!_config.open(QIODevice::WriteOnly)) {
        emit abort(QObject::tr("Could not open the config-file"));
        return;
    }
    QTextStream config(&_config);
    if(needTimeout) config << "timeout=0\n";
    config << (QString("title %2\n"
                       "find --set-root %1/kernel kernel "
                       "%1/kernel quiet root=/dev/ram0 "
                       "androidboot.hardware=android_x86 androidboot.selinux=permissive "
                       "SRC=%1 DATA=%1/data.img\n"
                       "find --set-root %1/initrd.img\n"
                       "initrd %1/initrd.img").arg(place, name));
    _config.close();
}

int install::sizeOfFiles() {
#if LINUX
    int res = QFile(mountPoint + "/initrd.img").size() +
            QFile(mountPoint + "/ramdisk.img").size() +
            QFile(mountPoint + "/kernel").size() +
            QFile(mountPoint + "/system.sfs").size() +
            QFile(mountPoint + "/system.img").size();
#elif WIN
    QString command = QString("%1/data/iso-editor.exe size %2 %3").arg(qApp->applicationDirPath(), systems.back().image);
    auto sizeOfFile = [&](QString file) -> int {
        auto expr = cmd::exec(command.arg(file));
        if(expr.first) {
            qWarning().noquote() << tr("Could not get size of %1").arg(file);
            return 200 * 1024;
        }
        else return expr.second.toInt();
    };
    int res = 0;
    QVector<QString> files { "system.img", "system.sfs", "ramdisk.img", "kernel", "initrd.img" };
    for(auto file : files) {
        res += sizeOfFile(file);
    }
#endif
    qDebug().noquote() << QObject::tr("Size of files is %1").arg(res);
    return res;
}

int install::isInvalidImage(
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
    return (systemImg || systemSfs) &&
            kernel && initrdImg &&
            ramdiskImg;
#endif
}

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
    if(!cmd::exec(QString("%1/data/iso-editor.exe exist %2 %3")
                  .arg(qApp->applicationDirPath(), systems.back().image, "system.img")).first)
#endif
        filesCopy.push_back((systemFile = "/system.img"));
#if LINUX
    else if(QFile(mountPoint + "/system.sfs").exists())
#elif WIN
    if(!cmd::exec(QString("%1/data/iso-editor.exe exist %2 %3")
                  .arg(qApp->applicationDirPath(), systems.back().image, "system.sfs")).first)
#endif
        filesCopy.push_back((systemFile = "/system.sfs"));
    qDebug().noquote() << QObject::tr("System file is %1").arg(systemFile);
    qDebug().noquote() << QObject::tr("Start copying");
    for(QString file : filesCopy) {

        if(file.isEmpty()) {
            emit abort(tr("File for copy is empty"));
            return;
        }
        qDebug().noquote() << QObject::tr("Copying %1").arg(file);
        if(QFile::exists(place + file)) {
            qDebug().noquote() << QObject::tr("%1 exists. So it is deleting").arg(place + file);
#if LINUX
            QString command = "rm -f %1";
#elif WIN
            QString command = "del /f /q %1";
#endif
            auto expr = cmd::exec(command.arg(place + file));
            if(expr.first) {
                emit logWindow(QtWarningMsg, QObject::tr("^Could not overwrite %1: %2").arg(place + file, expr.second));
            }
        }
        else qDebug().noquote() << QObject::tr("%1 does not exist").arg(place + file);
        bool res = 0;
#if LINUX
        res = copier.copy(mountPoint + file, place + file);
        QString advancedInfo = "";
#elif WIN
        QPair<int, QString> expr;
        res = !(expr = cmd::exec(QString("%1/data/iso-editor.exe extract %2 %3 %4/%3")
                                 .arg(qApp->applicationDirPath(), systems.back().image, file.remove(0, 1), place))).first;
        QString advancedInfo = QString(": %1").arg(expr.second);
#endif
        if(!res) {
            emit abort(QObject::tr("Could not copy %1-system file%2").arg(file, advancedInfo));
            return;
        }

        qDebug().noquote() << QObject::tr("%1 copied succesful").arg(file);
#if LINUX
        int size = QFile(mountPoint + file).size();
#elif WIN
        QString command = QString("%1/data/iso-editor.exe size %2 %3").arg(qApp->applicationDirPath(), systems.back().image);
        expr = cmd::exec(command.arg(file));
        int size = (!expr.first) ? expr.second.toInt() : 50;
#endif
        emit fileEnded(size);
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

void install::deleteBootloader(int numSys) {
    qDebug().noquote() << "Deleting bootloader...";
    switch(systems[numSys].bootloader) {
    case _bootloader::grub2: deleteGrub2(numSys); break;
    }
}

void install::deleteGrub2(int numSys) {
    qDebug().noquote() << "Deleting android from grub2...";
    statusBar->showMessage("Удаление android из grub2");
    QFile(qApp->translate("log", "/etc/grub.d/android/") + systems[numSys].name + ".cfg").remove();
    system("update-grub");
    progressBarDelete->setValue(7);
}

void install::deleteEntry(int num) {
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
    qDebug().noquote() << QObject::tr("Formating flash drive...");
    QString part = systems.back().place;

    QPair<int, QString> addNew;
    execAbort(QString("format %1 /q /v:android /fs:ntfs").arg(part));
    qDebug().noquote() << QObject::tr("Flash drive formated successfully...");
#endif
}

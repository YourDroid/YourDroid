#include "install.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "log.h"
#include "window.h"
#include "cmd.h"
#include <QErrorMessage>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QVector>
#include <bkisofs/bk.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>

#define ABORT() abort = 1; return;

void install::addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n) {
    systems.push_back(install::_installSet(b, t, p, i, n));
    cntSystems++;
}

void install::write() {
    bool sysDel;
    int sysCnt = _oldSysEdit ? (cntSystems) : 0;
    //cntSystems -= deletedSys.length();

    LOG(0, "Writing install settings...");

    QSettings install("install.ini", QSettings::IniFormat);
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", cntSystems);
    for(int i = 0; i < sysCnt; i++) {
        LOG(0, QString("System ") + QString::number(i + 1) + " config register...");
        install.setValue(QString("system_") + QString::number(i), systems[i].name);
        LOG(0, QString("System ") + QString::number(i + 1) + " config register succesfull");
    }
    install.endGroup();

    LOG(0, "System configs register succesfull");

    for(int i = 0; i < sysCnt; i++) {
        LOG(0, QString("System ") + QString::number(i + 1) + QString(" writing..."));
        QSettings system(systems[i].name + ".ini", QSettings::IniFormat);
        system.beginGroup("about");
        system.setValue("bootloader", _bootloaderHelper::to_string(systems[i].bootloader).c_str());
        system.setValue("type_place", _typePlaceHelper::to_string(systems[i].typePlace).c_str());
        system.setValue("place", systems[i].place);
        system.setValue("image", systems[i].image);
        system.setValue("os", systems[i].os);
        system.setValue("ended", systems[i].ended);
        system.endGroup();
        LOG(0, QString("System ") + QString::number(i + 1) + QString(" wrote succesfull"));
    }
    LOG(0, "Systems wrote succesfull");
}

void install::read() {
    LOG(0, "Reading install settings...");

    QSettings install("install.ini", QSettings::IniFormat);

    install.beginGroup("about_installing_systems");
    cntSystems = install.value("count_systems", 0).toInt();

    for(int i = 0; i < cntSystems; i++) {
        QString name = install.value(QString("system_") + QString::number(i), 0).toString();
        QString sys = name + ".ini";
        LOG(0, /*install.value(QString("system_") + QString::number(i), 0).toString()*/name);
        if(!QFile::exists(sys)) {
            LOG(2,
                         QString("Config of system ") + name + " does not exist",
                         QString("Файл настроек системы ") + name + " не существует!");
            _oldSysEdit = true;
            cntSystems = cntSystems - 1;
            continue;
        }
        QSettings system(sys, QSettings::IniFormat);
        system.beginGroup("about");
        if(!system.contains("bootloader")) {
            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section bootloader", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о загрузчике");
        }
        else if(!system.contains("type_place")) {
            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section type_place", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о типе установки(дериктория/раздел)");
        }
        else if(!system.contains("place")) {
            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section place", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о месте установки");
        }
//        else if(!system.contains("name")) {
//            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section name", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о имени системы");
//        }
        if(!system.contains("os")) {
            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section os", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о системе, на которой устанавливался андроид");
        }
        if(!system.contains("ended")) {
            LOG(2, QString("Config of system ") + QString::number(i + 1) + " does not have section ended", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о успешной установке");
        }
        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", "Gummiboot").toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", "Error of read!").toString().toStdString());
        QString place = system.value("place", "Error of read!").toString();
        QString image = system.value("image", "Error of read!").toString();
        bool os = system.value("os", false).toBool();
        bool ended = system.value("ended", false).toBool();
        LOG(0, QString("System ") + QString::number(i + 1) + " read succesfull");
        systems.push_back(_installSet(boot, typePlace, place, image, name, ended, os));
        LOG(0, QString("System ") + QString::number(i + 1) + " push succesfull");
        system.endGroup();
    }

    install.endGroup();

    LOG(0, "Systems read succesfull");
}

void install::registerBootloader() {
    LOG(0, "Registering to bootloader...");
    switch(systems.back().bootloader) {
    case _bootloader::grub2: LOG(0, "Registering to grub2"); registerGrub2(); break;
    case _bootloader::gummiboot: LOG(0, "Registering to gummiboot"); registerGummi(); break;
    }
}

void install::registerGummi() {
    if(!isInstalledGummi()) installGummi();
    return;
    cmd _cmd;
    char i[2] = {'a', '\0'};
    for(; i[0] < 123 && QDir(QString(i) + ":/").exists(); i[0]++);
    QString symbol = i;
    grubConfigure(WORK_DIR + "/tempGrubConf");
    QVector<QString> commands = { QString("mountvol ") + symbol + QString(": /s"),        //1
                                QString("mkdir ") + symbol + ":/EFI/yourdroid_gummiboot", //2
                                QString("cp ") +
                                (WORK_DIR + "/data/bootloaders/gummi/") +
                                  (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi") +
                                  QString(" ") + symbol +
                                  QString(":/EFI/yourdroid_gummiboot/"),                  //3
                                QString("mk ") + symbol + ":/loader",                     //4
                                QString("cp ") + WORK_DIR +
                                  "/data/bootloaders/gummi/loader/loader.conf "
                                  "A:/loader/loader.conf",                                //5
                                "mk A:/loader/entries",                                   //6
                                QString("cp ") + WORK_DIR +
                                  "/data/bootloaders/gummi/loader/entries/0windows.conf "
                                  "A:/loader/entries/0windows.conf",                      //7
                                QString("cp ") + WORK_DIR +
                                  QString("/tempGrubConf ") +
                                  QString("A:/loader/entries/") +
                                  systems.back().name + ".conf",                          //8
                                QString("mountvol a: /d"                                          //9
                                "bcdedit /set {bootmgr} path "
                                  "/EFI/yourdroid_gummiboot/") +
                                  (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi"),    //10
                                "bcdedit /set {bootmgr} description "
                                  "\"YourDroid Gummiboot\""                               //11
                                };                                                        //***
    for(int i = 0; i < 9; i++) {

//        if(_cmd.exec(commands[i]).first) {
//            LOG(2, QString("Fatal Error: ") + _cmd.output(), "Критическая ошибка: " + _cmd.output());
//            ABORT();
//        }
    }
}

bool install::isInstalledGummi() {
    LOG(0, "Checking gummiboot");
    cmd::exec(QString("bcdedit /enum firmware > ") + WORK_DIR + "/tempEnum");
    bool res = !cmd::exec(QString("find \"YourDroid Gummiboot\" <") + WORK_DIR + "/tempEnum").first;
    LOG(0, res ? "Gummiboot installed" : "Gummiboot did not installed");
    return res;
}

void install::installGummi() {
    LOG(0, "Installing gummiboot...");
#define CHECK_ABORT() if(addNew.first)  { \
    LOG(2, QString("Fatal Error: ") + addNew.second, "Критическая ошибка: " + addNew.second); ABORT(); }

#define execAbort(command) addNew = cmd::exec(command); CHECK_ABORT();

    QPair<int, QString> addNew;
    execAbort("bcdedit /copy {bootmgr} /d \"YourDroid Gummiboot\"");

    QString output = addNew.second;
    int begin = output.indexOf('{'), end = output.indexOf('}');
    QString id = output.mid(begin, end);

    execAbort(QString("/set ") + id + QString(" path /EFI/yourdroid_gummiboot/") +
                       (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi"));

    char i[2] = {'a', '\0'};
    for(; i[0] < 123 && QDir(QString(i) + ":/").exists(); i[0]++);
    QString s = i;

    execAbort(QString("mountvol") + s + QString(": /s"));

    execAbort(QString("mkdir ") + s + ":/EFI/yourdroid_gummiboot");

    execAbort(QString("cp ") +
                       (WORK_DIR + "/data/bootloaders/gummi/") +
                       (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi") +
                       QString(" ") + s +
                       QString(":/EFI/yourdroid_gummiboot/"));

    execAbort(QString("mkdir ") + s + ":/loader");


    execAbort(QString("cp ") + WORK_DIR +
              QString("/data/bootloaders/gummi/loader/loader.conf ") + s +
              ":/loader/loader.conf");

    execAbort(QString("cp ") + WORK_DIR +
              QString("/data/bootloaders/gummi/loader/entries/0windows.conf ") + s +
              ":/loader/entries/0windows.conf");

    execAbort(QString("mkdir ") + s +
              QString(":/efi/grub"));

    execAbort(QString("mkdir ") + s +
              QString(":/efi/grub/"));

    LOG(0, "Gummiboot installed succes");

#undef CHECK_ABORT
}

void install::registerGrub2() {
    LOG(0, "Registering to grub2...");
    using namespace std;
    ostringstream tempGrub;
    fstream grub("/etc/grub.d/40_custom", ios::in | ios::out);
    QVector<string> grubStr;
    while(grub) {
        string temp;
        getline(grub, temp);
        grubStr.push_back(temp);
    }
    for(int i = 0; i < grubStr.length(); i++) LOG(0, QString::fromStdString(grubStr[i]));
    if(grubStr[1] != "cat /etc/grub.d/android/*.cfg | more") {
        tempGrub << grubStr[0] << "\ncat /etc/grub.d/android/*.cfg | more\n";
        for(int i = 1; i < grubStr.length(); i++) tempGrub << grubStr[i] << '\n';
        ofstream("/etc/grub.d/40_custom") << tempGrub.str();
        QDir().mkdir("/etc/grub.d/android");
        //system("mkdir /etc/grub.d/android");
    }
    grubConfigure(QString("/etc/grub.d/android/") + systems.back().name + ".cfg");
    if(cmd::exec("update-grub").first) ABORT();
}

void install::grubConfigure(QString way) {
    QString place = systems.back().name;
#if OS == 1
    place = place.left(1);
#endif
    QFile _config(way);
    if(!_config.open(QIODevice::WriteOnly)) ABORT();
    QTextStream config(&_config);
    config << (QString("menuentry '") + place + QString("' --class android-x86 {\n") +
           QString("\tsearch --file --no-floppy --set=root ") + place +  QString("/kernel\n") +
           QString("\tlinux ") + place + QString("/kernel root=/dev/ram0 androidboot.hardware=android-x86 androidboot.selinux=permissive\n") +
           QString("\tinitrd ") + place + QString("/initrd.img\n") + "}\n");
    _config.close();
}

void install::unpackSystem() {
    int rc = 0;
    auto checkRc = [](int rc) -> void {
        QString error;
        if(rc <= 0 && rc != -1026) {
#if OS == 0
            error = bk_get_error_string(rc);
#elif OS == 1
#endif
            LOG(2, QString("Error while unpacking image: ") + error, QString("Ошибка при разархивировании: #") + QString::number(rc) + QString(' ') + error);
        }
    };
#if OS == 0
    VolInfo volInfo;
    checkRc(bk_init_vol_info(&volInfo, false));
    checkRc(bk_open_image(&volInfo, systems.back().image.toStdString().c_str()));
    checkRc(bk_read_vol_info(&volInfo));
    if(volInfo.filenameTypes & FNTYPE_ROCKRIDGE)
            rc = bk_read_dir_tree(&volInfo, FNTYPE_ROCKRIDGE, true, 0);
    else if(volInfo.filenameTypes & FNTYPE_JOLIET)
            rc = bk_read_dir_tree(&volInfo, FNTYPE_JOLIET, false, 0);
    else
            rc = bk_read_dir_tree(&volInfo, FNTYPE_9660, false, 0);
    checkRc(rc);
    char *files[5] = {"/kernel", "/initrd.img", "/ramdisk.img", "/system.img", "/system.sfs"};
        for(int i = 0; i < 5; i++) {
            progressBarInstall->setValue(progressBarInstall->value() + 25);
            statusBar->showMessage(QString("Распаковывается ") + files[i]);
            rc = bk_extract(&volInfo, files[i],
                            systems.back().place.toStdString().c_str(),
                            false,
                    0);
        checkRc(rc);
        }

        /* we're finished with this ISO, so clean up */
        bk_destroy_vol_info(&volInfo);
#elif OS == 1
#endif
}

void install::createDataImg(int size) {
#if OS == 0
    system((QString("chmod 777 ") + WORK_DIR + "/data/make_ext4fs/make_ext4fs").toStdString().c_str());
    QString command = WORK_DIR + QString("/data/make_ext4fs/make_ext4fs") + QString(" -l ") + QString::number(size) +
                       QString("M -a data ") + systems.back().place + QString("/data.img ");
#elif OS == 1
    QString command = WORK_DIR + QString("/data/make_ext4fs/make_ext4fs.exe") + QString(" -l ") + QString::number(size) +
                       QString("M -a data ") + systems.back().place + QString("/data.img ");
#endif
    auto res = cmd().exec(command);
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
    LOG(0, "Deleting system files...");
    QVector<QString> files = {"/kernel", "/initrd.img", "/ramdisk.img", "/system.img", "/system.sfs", "/data.img"};
    for(auto file : files) {
        progressBarDelete->setValue(progressBarDelete->value() + 1);
        LOG(0, QString("Delete ") + systems[numSys].place + file);
        statusBar->showMessage(QString("Удаление ") + file);
        QFile(systems[numSys].place + file).remove();
    }
}

void install::execBars(QProgressBar *progressins, QProgressBar *progressdel, QStatusBar *status) {
    LOG(0, "Exec progress and status bars");
    progressBarInstall = progressins;
    progressBarDelete = progressdel;
    statusBar = status;
}

void install::deleteBootloader(int numSys) {
    LOG(0, "Deleting bootloader...");
    switch(systems[numSys].bootloader) {
    case _bootloader::grub2: deleteGrub2(numSys); break;
    }
}

void install::deleteGrub2(int numSys) {
    LOG(0, "Deleting android from grub2...");
    statusBar->showMessage("Удаление android из grub2");
    QFile(QString("/etc/grub.d/android/") + systems[numSys].name + ".cfg").remove();
    system("update-grub");
    progressBarDelete->setValue(7);
}

void install::deleteEntry(int num) {
    LOG(0, "Deleting system's entry...");
    systems.remove(num);
    cntSystems--;
    LOG(0, "System's entry deleted succesfull");
    LOG(0, "Deleting system's config...");
    bool goodDel = QFile(WORK_DIR + QString('/') + systems[num].name + ".ini").remove();
    LOG(!goodDel * 2, QString("Config deleted ") + (goodDel ? "succesfully" : "unsuccesfully!"));
}

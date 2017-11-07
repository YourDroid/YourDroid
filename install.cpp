#include "install.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "log.h"
#include "window.h"
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

void install::addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n) {
    systems.push_back(install::_installSet(b, t, p, i, n));
    cntSystems++;
}

void install::write() {
    bool sysDel;
    int sysCnt = _oldSysEdit ? (cntSystems) : 0;
    //cntSystems -= deletedSys.length();

    log::message(0, __FILE__, __LINE__, "Writing install settings...");

    QSettings install("install.ini", QSettings::IniFormat);
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", cntSystems);
    for(int i = 0; i < sysCnt; i++) {
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " config register...");
        install.setValue(QString("system_") + QString::number(i), systems[i].name);
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " config register succesfull");
    }
    install.endGroup();

    log::message(0, __FILE__, __LINE__, "System configs register succesfull");

    for(int i = 0; i < sysCnt; i++) {
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + QString(" writing..."));
        QSettings system(systems[i].name + ".ini", QSettings::IniFormat);
        system.beginGroup("about");
        system.setValue("bootloader", _bootloaderHelper::to_string(systems[i].bootloader).c_str());
        system.setValue("type_place", _typePlaceHelper::to_string(systems[i].typePlace).c_str());
        system.setValue("place", systems[i].place);
        system.setValue("image", systems[i].image);
        system.setValue("os", systems[i].os);
        system.setValue("ended", systems[i].ended);
        system.endGroup();
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + QString(" wrote succesfull"));
    }
    log::message(0, __FILE__, __LINE__, "Systems wrote succesfull");
}

void install::read() {
    log::message(0, __FILE__, __LINE__, "Reading install settings...");

    QSettings install("install.ini", QSettings::IniFormat);

    install.beginGroup("about_installing_systems");
    cntSystems = install.value("count_systems", 0).toInt();

    for(int i = 0; i < cntSystems; i++) {
        QString name = install.value(QString("system_") + QString::number(i), 0).toString();
        QString sys = name + ".ini";
        log::message(0, __FILE__, __LINE__, /*install.value(QString("system_") + QString::number(i), 0).toString()*/name);
        if(!QFile::exists(sys)) {
            log::message(2, __FILE__, __LINE__,
                         QString("Config of system ") + name + " does not exist",
                         QString("Файл настроек системы ") + name + " не существует!");
            _oldSysEdit = true;
            cntSystems = cntSystems - 1;
            continue;
        }
        QSettings system(sys, QSettings::IniFormat);
        system.beginGroup("about");
        if(!system.contains("bootloader")) {
            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section bootloader", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о загрузчике");
        }
        else if(!system.contains("type_place")) {
            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section type_place", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о типе установки(дериктория/раздел)");
        }
        else if(!system.contains("place")) {
            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section place", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о месте установки");
        }
//        else if(!system.contains("name")) {
//            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section name", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о имени системы");
//        }
        if(!system.contains("os")) {
            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section os", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о системе, на которой устанавливался андроид");
        }
        if(!system.contains("ended")) {
            log::message(2, __FILE__, __LINE__, QString("Config of system ") + QString::number(i + 1) + " does not have section ended", QString("В настройках системы ") + QString::number(i + 1) + " отсутствует секция о успешной установке");
        }
        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", "Gummiboot").toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", "Error of read!").toString().toStdString());
        QString place = system.value("place", "Error of read!").toString();
        QString image = system.value("image", "Error of read!").toString();
//        QString name = install.value(QString("system_") + QString::number(i), 0).toString();
        bool os = system.value("os", false).toBool();
        bool ended = system.value("ended", false).toBool();
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " read succesfull");
        systems.push_back(_installSet(boot, typePlace, place, image, name, ended, os));
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " push succesfull");
        system.endGroup();
    }

    install.endGroup();

    log::message(0, __FILE__, __LINE__, "Systems read succesfull");
}

void install::installBootloader() {
    log::message(0, __FILE__, __LINE__, "Registering to bootloader...");
    switch(systems.back().bootloader) {
    case _bootloader::grub2: installGrub2(); break;
    case _bootloader::gummiboot: installGummi(); break;
    }
}

void install::installGummi() {
    using namespace std;
    auto copy = [](string src, string dest){
        ifstream source(src, ios::binary);
        ofstream destination(dest, ios::binary);
        if(!source) log::message(2, __FILE__, __LINE__, QString("Can not read ") + QString::fromStdString(src), "Невозможно прочитать " + QString::fromStdString(src));
        if(!destination) log::message(2, __FILE__, __LINE__, QString("Can not write ") + QString::fromStdString(dest), "Невозможно записать " + QString::fromStdString(dest));
        for(char ch; source;) {
            source.read((char*)&ch,sizeof(char));
            destination.write((char*)&ch,sizeof(char));
        }
    };

    system("mountvol b: /s");
    //QFile::rename("A:\\EFI\\Microsoft\\Boot\\bootmgfw.efi", "A:\\EFI\\Microsoft\\Boot\\bootmgfw_back.efi");
    system("cp B:/EFI/Microsoft/Boot/bootmgfw.efi B:/EFI/Microsoft/Boot/bootmgfw_back.efi");
    //copy(QString((workDir + "/data/bootloaders\\gummi\\") + (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi")).toStdString(), "A:\\EFI\\Microsoft\\Boot\\bootmgfw.efi");
    system(QString(QString("cp ") +
                   (workDir + "/data/bootloaders/gummi/") + (dat->arch ? "gummiboot64.efi" : "gummiboot32.efi") +
                   QString(" A:/EFI/Microsoft/Boot/bootmgfw.efi")).toStdString().c_str());
    //QDir().mkdir("A:\\loader");
    system("mk A:/loader");
    //copy(QString(workDir + "\\data\\bootloaders\\gummi\\loader\\" + "loader.conf").toStdString(), "A:\\loader\\loader.conf");
    system(QString(QString("cp ") + workDir + "/data/bootloaders/gummi/loader/loader.conf A:/loader/loader.conf").toStdString().c_str());
    //QDir().mkdir("A:\\loader\\entries");
    system("mk A:/loader/entries");
    //copy(QString(workDir + "\\data\\bootloaders\\gummi\\loader\\entries\\" + "0windows.conf").toStdString(), "A:\\loader\\entries\\0windows.conf");
    system(QString(QString("cp ") + workDir + "/data/bootloaders/gummi/loader/entries/0windows.conf A:/loader/entries/0windows.conf").toStdString().c_str());
    grubConfigure(workDir + "/tempGrubConf");
    system((QString("cp ") + workDir + QString("/tempGrubConf ") + QString("A:/loader/entries/") + systems.back().name + ".conf").toStdString().c_str());
    system("mountvol a: /d");
}

void install::installGrub2() {
    log::message(0, __FILE__, __LINE__, "Registering to grub2...");
    using namespace std;
    ostringstream tempGrub;
    fstream grub("/etc/grub.d/40_custom", ios::in | ios::out);
    QVector<string> grubStr;
    while(grub) {
        string temp;
        getline(grub, temp);
        grubStr.push_back(temp);
    }
    for(int i = 0; i < grubStr.length(); i++) log::message(0, __FILE__, __LINE__, QString::fromStdString(grubStr[i]));
    if(grubStr[1] != "cat /etc/grub.d/android/*.cfg | more") {
        tempGrub << grubStr[0] << "\ncat /etc/grub.d/android/*.cfg | more\n";
        for(int i = 1; i < grubStr.length(); i++) tempGrub << grubStr[i] << '\n';
        ofstream("/etc/grub.d/40_custom") << tempGrub.str();
        QDir().mkdir("/etc/grub.d/android");
        //system("mkdir /etc/grub.d/android");
    }
    grubConfigure(QString("/etc/grub.d/android/") + systems.back().name + ".cfg");
    system("update-grub");
}

void install::grubConfigure(QString way) {
    std::ofstream config(way.toStdString());
    config << "menuentry '" << systems.back().name.toStdString() << "' --class android-x86 {\n"
           << "\tsearch --file --no-floppy --set=root " << systems.back().place.toStdString() << "/kernel\n"
           << "\tlinux " << systems.back().place.toStdString() << "/kernel root=/dev/ram0 androidboot.hardware=android-x86 androidboot.selinux=permissive\n"
           << "\tinitrd " << systems.back().place.toStdString() << "/initrd.img\n"
           << "}\n";
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
            log::message(2, __FILE__, __LINE__, error, QString("Ошибка при разархивировании: #") + QString::number(rc) + QString(' ') + error);
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
    system((QString("chmod 777 ") + workDir + "/data/make_ext4fs/make_ext4fs").toStdString().c_str());
    QString command = workDir + QString("/data/make_ext4fs/make_ext4fs") + QString(" -l ") + QString::number(size) +
                       QString("M -a data ") + systems.back().place + QString("/data.img ");
#elif OS == 1
    QString command = workDir + QString("/data/make_ext4fs/make_ext4fs") + QString(" -l ") + QString::number(size) +
                       QString("M -a data ") + systems.back().place + QString("/data.img ");
#endif
    LOG(0, command)
    system(command.toStdString().c_str());
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
           log::message(2, __FILE__, __LINE__, err, err);
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
    log::message(0, __FILE__, __LINE__, "Deleting bootloader...");
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
    bool goodDel = QFile(workDir + QString('/') + systems[num].name + ".ini").remove();
    LOG(!goodDel * 2, QString("Config deleted ") + (goodDel ? "succesfully" : "unsuccesfully!"));
}

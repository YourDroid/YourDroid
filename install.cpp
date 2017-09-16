#include <install.h>
#include <string>
#include <zlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "log.h"
#include <QErrorMessage>
#include <QFile>
#include <QDebug>
#include <functional>

void install::addSystem(_bootloader b, _typePlace t, QString p, QString n) {
    systems.push_back(install::_installSet(b, t, p, n));
    cntSystems++;
}

void install::write() {
    int sysCnt = _oldSysEdit ? (cntSystems - 1) : 0;

    log::message(0, __FILE__, __LINE__, "Writing install settings...");

    QSettings install("install.ini", QSettings::IniFormat);
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", cntSystems);
    for(int i = sysCnt; i < cntSystems; i++) {
        install.setValue(QString("system_") + QString::number(i), systems[i].name);
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " config register succesfull");
    }
    install.endGroup();

    log::message(0, __FILE__, __LINE__, "System configs register succesfull");

    for(int i = sysCnt; i < cntSystems; i++) {
        QSettings system(systems[i].name + ".ini", QSettings::IniFormat);
        system.beginGroup("about");
        system.setValue("bootloader", _bootloaderHelper::to_string(systems[i].bootloader).c_str());
        system.setValue("type_place", _typePlaceHelper::to_string(systems[i].typePlace).c_str());
        system.setValue("place", systems[i].place);
        system.setValue("name", systems[i].name);
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
        QString sys = install.value(QString("system_") + QString::number(i), 0).toString() + ".ini";
        auto error = [](QString logErr, QString messErr) -> void {
            QErrorMessage mess;
            log::message(2, __FILE__, __LINE__, logErr);
            mess.showMessage(messErr);
            mess.exec();
        };
        if(!QFile::exists(sys)) {
            QErrorMessage mess;
            log::message(1, __FILE__, __LINE__, QString("System ") + install.value(QString("config of system ") + QString::number(i + 1), 0).toString() + " does not exist");
            mess.showMessage(QString("Файл настроек системы ") + install.value(QString("Настройки системы ") + QString::number(i + 1), 0).toString() + " не существует!");
            mess.exec();
            _oldSysEdit = true;
            cntSystems = cntSystems - 1;
            continue;
        }
        QSettings system(sys, QSettings::IniFormat);
        system.beginGroup("about");
        if(!system.contains("bootloader")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section bootloader", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о загрузчике");
        }
        else if(!system.contains("type_place")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section type_place", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о типе установки(дериктория/раздел)");
        }
        else if(!system.contains("place")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section place", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о месте установки");
        }
        else if(!system.contains("name")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section name", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о имени системы");
        }
        if(!system.contains("os")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section os", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о системе, на которой устанавливался андроид");
        }
        if(!system.contains("ended")) {
            error(QString("Config of system ") + QString::number(i + 1) + " does not have section ended", QString("Ошибка!!!\nВ настройках системы ") + QString::number(i + 1) + " отсутствует секция о успешной установке");
        }
        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", "Gummiboot").toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", "Error of read!").toString().toStdString());
        QString place = system.value("place", "Error of read!").toString();
        QString name = system.value("name", "Error of read!").toString();
        bool os = system.value("os", false).toBool();
        bool ended = system.value("ended", false).toBool();
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " read succesfull");
        systems.push_back(_installSet(boot, typePlace, place, name, ended, os));
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " push succesfull");
        system.endGroup();
    }

    install.endGroup();

    log::message(0, __FILE__, __LINE__, "Systems read succesfull");
}

void install::installBootloader() {
    log::message(0, __FILE__, __LINE__, "Registering to bootloader...");
    switch(systems.back().bootloader) {
    case _bootloader::Grub2: installGrub2(); break;
    case _bootloader::Gummiboot: installGummi(); break;
    }
}

void install::installGummi() {

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
    if(grubStr[1] != "cat ./android/*.cfg") {
        tempGrub << grubStr[0] << "\ncat ./android/*.cfg\n";
        for(int i = 1; i < grubStr.length(); i++) tempGrub << grubStr[i] << '\n';
        ofstream("/etc/grub.d/40_custom") << tempGrub.str();
        system("mkdir /etc/grub.d/android");
    }
    grubConfigure(QString("/etc/grub.d/android/") + QString::number(cntSystems) + systems.back().name + ".cfg");
}

void install::grubConfigure(QString way) {
    std::ofstream config(way.toStdString());
    config << "menuentry '" << systems.back().name.toStdString() << "' --class android-x86 {\n"
           << "\tsearch --file --no-floppy --set=root " << systems.back().place.toStdString() << "/kernel\n"
           << "\tlinux " << systems.back().place.toStdString() << "/kernel root=/dev/ram0 androidboot.hardware=android-x86 androidboot.selinux=permissive\n"
           << "\tinitrd " << systems.back().place.toStdString() << "/initrd.img\n"
           << "}\n";
}






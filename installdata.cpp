#include <installdata.h>
#include <string>

#include "log.h"

void installData::addSystem(_bootloader b, _typePlace t, QString p, QString n) {
    systems.push_back(installData::_installSet(b, t, p, n));
    cntSystems++;
}

void installData::write() {

    log::message(0, __FILE__, __LINE__, "Writing install settings...");

    QSettings install("install.ini", QSettings::IniFormat);
    install.beginGroup("about_installing_systems");
    install.setValue("count_systems", systems.length());
    for(int i = 0; i < systems.length(); i++) {
        install.setValue(QString("system_") + char(i + 48), systems[i].name);
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " config register succesfull");
    }
    install.endGroup();

    log::message(0, __FILE__, __LINE__, "System configs register succesfull");

    for(int i = 0; i < systems.length(); i++) {
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

void installData::read() {
    log::message(0, __FILE__, __LINE__, "Reading install settings...");

    QSettings install("install.ini", QSettings::IniFormat);

    install.beginGroup("about_installing_systems");
    int cntSystems = install.value("count_systems", 0).toInt();

    for(int i = 0; i < cntSystems; i++) {
        QSettings system(install.value(QString("system_") + char(i + 49), 0).toString(), QSettings::IniFormat);
        system.beginGroup("about");
        _bootloader boot;
        boot = _bootloaderHelper::from_string(system.value("bootloader", ' ').toString().toStdString());
        _typePlace typePlace;
        typePlace = _typePlaceHelper::from_string(system.value("type_place", ' ').toString().toStdString());
        QString place = system.value("place", ' ').toString();
        QString name = system.value("name", ' ').toString();
        bool os = system.value("os", ' ').toBool();
        bool ended = system.value("ended", ' ').toBool();
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " read succesfull");
        systems.push_back(_installSet(boot, typePlace, place, name, ended, os));
        log::message(0, __FILE__, __LINE__, QString("System ") + QString::number(i + 1) + " push succesfull");
    }

    install.endGroup();

    log::message(0, __FILE__, __LINE__, "Systems read succesfull");
}


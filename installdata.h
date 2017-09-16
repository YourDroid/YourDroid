#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include "enum.h"
#include "os.h"

STRING_ENUM(_bootloader, Grub2, Grub_legasy, rEFInd, Gummiboot, NTLDR, BOOTMGR)
STRING_ENUM(_typePlace, dir, partition)

class installData {
    struct _installSet {
        _bootloader bootloader;
        _typePlace typePlace;
        QString place;
        QString name;
        bool ended = false;
        bool os = OS;
        _installSet(_bootloader b, _typePlace t, QString p, QString n) : bootloader(b), typePlace(t), place(p), name(n) {}
        _installSet() {}
        _installSet(_bootloader b, _typePlace t, QString p, QString n, bool e, bool o) : bootloader(b), typePlace(t), place(p), name(n), ended(e), os(o) {}
    };
public:
    QVector<installData::_installSet> systems;
    int cntSystems;
    void addSystem(_bootloader b, _typePlace t, QString p, QString n);
    void read();
    void write();

};

#endif // INSTALLDATA_H

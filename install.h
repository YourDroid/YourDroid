#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include <QProgressBar>
#include "enum.h"
#include "os.h"

STRING_ENUM(_bootloader, Grub2, Grub_legasy, rEFInd, Gummiboot, NTLDR, BOOTMGR)
STRING_ENUM(_typePlace, dir, partition)

class install {
    struct _installSet {
        _bootloader bootloader;
        _typePlace typePlace;
        QString place;
        QString image;
        QString name;
        bool ended = false;
        bool os = OS;
        _installSet(_bootloader b, _typePlace t, QString p, QString i, QString n) : bootloader(b), typePlace(t), place(p), image(i), name(n) {}
        _installSet() {}
        _installSet(_bootloader b, _typePlace t, QString p, QString i, QString n, bool e, bool o) : bootloader(b), typePlace(t), place(p), image(i), name(n), ended(e), os(o) {}
    };
    QVector<install::_installSet> systems;
    int cntSystems = 0;
    bool _oldSysEdit = false;
public:
    const QVector<install::_installSet>& systemsVector() { return systems; }
    int cntSys() { return cntSystems; }
    bool &oldSysEdit() { return _oldSysEdit; }
    void addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n);
    void read();
    void write();

    void unpackSystem(QProgressBar*);
    void installBootloader();
    void installGrub2();
    void grubConfigure(QString);
    void installGummi();

};

#endif // INSTALLDATA_H

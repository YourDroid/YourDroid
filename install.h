#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include <QProgressBar>
#include <QStatusBar>
#include "enum.h"
#include "data.h"
#include <bkisofs/bk.h>

STRING_ENUM(_bootloader, grub2, grub_legasy, refind, gummiboot, ntldr, bootmgr)
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
    options *dat;
    QString workDir;
public:
    install(QString workdir, options *d) : workDir(workdir), dat(d) {}
    const QVector<install::_installSet>& systemsVector() { return systems; }
    int cntSys() { return cntSystems; }
    bool &oldSysEdit() { return _oldSysEdit; }
    void addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n);
    void read();
    void write();

    void unpackSystem(QProgressBar*, QStatusBar*);
    void installBootloader();
    void installGrub2();
    void grubConfigure(QString);
    void installGummi();
    void createDataImg(int);
    void downloadFile(QString, QString);

};

#endif // INSTALLDATA_H

#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include <QProgressBar>
#include <QStatusBar>
#include "enum.h"
#include "data.h"

STRING_ENUM(_bootloader, grub2, grub_legasy, refind, gummiboot, ntldr, bootmgr)
STRING_ENUM(_typePlace, dir, partition)

extern const QString &WORK_DIR;

class install : public QObject {
    Q_OBJECT
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
    QProgressBar *progressBarInstall;
    QProgressBar *progressBarDelete;
    QStatusBar *statusBar;
    QVector<int> deletedSys;
    QString abortMes;
    QString mountPoint;
signals:
    void abort(QString);
    void progressChange(qint64);
    void fileEnded(int);
public:
    install(options *d) : dat(d) {}
    const QVector<install::_installSet>& systemsVector() { return systems; }
    int cntSys() { return cntSystems; }
    bool &oldSysEdit() { return _oldSysEdit; }
    QVector<int> &deletedSystems() { return deletedSys; }
    void addSystem(_bootloader b, _typePlace t, QString p, QString i, QString n);
    void read();
    void write();
    void execBars(QProgressBar*, QProgressBar*, QStatusBar*);

    int sizeOfFiles();
    bool isInvalidImage();
    QPair<bool, QString> mountImage(QString);
    void unmountImage();
    void unpackSystem();
    QString obsolutePath(QString);
    void registerBootloader();
    void registerGrub2();
    void grubConfigure(QString);
    void registerGummi();
    bool isInstalledGummi();
    void installGummi();
    void createDataImg(int);
    void downloadFile(QString, QString);
    void delSystemFiles(int);
    void deleteBootloader(int);
    void deleteGrub2(int);
    void deleteEntry(int);

};

#endif // INSTALLDATA_H

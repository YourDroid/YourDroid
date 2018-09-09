#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include <QProgressBar>
#include <QStatusBar>
#include "3rdparty/enum.h"
#include "3rdparty/tagged_bool.h"
#include "data.h"

using install2Flash = tagged_bool<class install2FlashTag>;

STRING_ENUM(_bootloader, grub2, grub_legasy, refind, gummiboot, ntldr, bootmgr, grub2_flash)
STRING_ENUM(_typePlace, dir, partition, flash_drive)

class install : public QObject {
    Q_OBJECT
    struct _installSet {
        _bootloader bootloader;
        _typePlace typePlace;
        QString place;
        QString image;
        QString name;
        QString bcdId;
        bool ended = false;
        bool os = OS;
        //_installSet(_bootloader b, _typePlace t, QString p, QString i, QString n, bool e) : bootloader(b), typePlace(t), place(p), image(i), name(n), ended(e) {}
        _installSet() {}
        _installSet(_bootloader b, _typePlace t, QString p, QString i, QString n, bool e, bool o = OS, QString bId = "") : bootloader(b), typePlace(t), place(p), image(i), name(n), ended(e), os(o), bcdId(bId) {}
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
    QPair<int, QString> resCmd;
    bool resFile = false;
signals:
    void logWindow(QtMsgType, QString);
    void abort(QString);
    void progressChange(qint64);
    void fileEnded(int);
public:
    install(options *d) : dat(d) {}
    const QVector<install::_installSet>& systemsVector() { return systems; }
    int cntSys() { return cntSystems; }
    bool &oldSysEdit() { return _oldSysEdit; }
    QVector<int> &deletedSystems() { return deletedSys; }
    void addSystem(_bootloader, _typePlace, QString, QString, QString, bool);
    void delBackSystem() { systems.removeLast(); }
    void systemEnd() { systems.back().ended = true; }
    void read();
    void write();
    void execBars(QProgressBar*, QProgressBar*, QStatusBar*);

    void formatFlashDrive();
    int sizeOfFiles();
    int isInvalidImage(
        #if WIN
            QString
        #endif
            );
    QPair<bool, QString> mountImage(QString);
    void unmountImage();
    void unpackSystem();
    QString obsolutePath(QString);
    void registerBootloader();
    void registerBootmgr();
    void registerGrub2();
    bool installGrub2( );
    QString grub2Configure(QString, bool = false, bool = true);
    void grubLConfigure(QString, bool = false);
    void createDataImg(int);
    void downloadFile(QString, QString);
    void delSystemFiles(int);
    void deleteBootloader(int);
    void deleteGrub2(int);
    void deleteEntry(int);

};

#if WIN
#define checkAbortCmd() if(resCmd.first)  { emit abort(resCmd.second); returnFault(); }

#define checkAbortFile() if(!resFile) { emit abort(QObject::tr("Couldn't copy needed files")); returnFault(); }

#define COPY(src, dst) resFile = _copy(src, dst); checkAbortFile();

#define MKDIR(name) resFile = _mkdir(name); checkAbortFile();

#define RENAME(src, dst) resFile = _rename(src, dst); checkAbortFile();

#define execAbort(command) resCmd = cmd::exec(command, true); checkAbortCmd();

#define logDirExist() qDebug().noquote() \
    << QString("%1 %2").arg(path, (res ? QObject::tr("exists") : QObject::tr("doesn't exist")));
#endif

#endif // INSTALLDATA_H

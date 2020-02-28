#ifndef INSTALLDATA_H
#define INSTALLDATA_H

#include <QString>
#include <QVector>
#include <QSettings>
#include <QProgressBar>
#include <QStatusBar>
#include <QCoreApplication>
#include "3rdparty/enum.h"
#include "3rdparty/tagged_bool.h"
#include "options.h"

using install2Flash = tagged_bool<class install2FlashTag>;

STRING_ENUM(_bootloader, grub2, grub4dos, refind, gummiboot, win_bootloader, grub2_flash, grub2_tablet)
STRING_ENUM(_typePlace, dir, partition, flash_drive)
enum sysImgExtractType {toImg = 0, toSfs = 1, toFolder = 2};

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
    QString settingsPath = QCoreApplication::instance()->applicationDirPath() + "/config";
    bool _oldSysEdit = false;
    options *dat;
    QProgressBar *progressBarInstall;
    QProgressBar *progressBarDelete;
    QStatusBar *statusBar;
    QVector<int> deletedSys;
    QString abortMes;
    QString mountPoint;
    QString usbBootPart;
    QString usbMainPart;
    qint64 usbMainSize = 0;
    int usbDiskIndex = 0;
    bool usbAlreadyFormatted = false;
    QPair<int, QString> resCmd;
    bool resFile = false;
    bool sysTypeSfs; // img - false, sfs - true
signals:
    void logWindow(QtMsgType, QString);
    void abort(QString);
    void progressChange(qint64);
    void fileEnded(int);
public:
    install(options *d) : dat(d) {}
    const QVector<install::_installSet>& systemsVector() { return systems; }
    int cntSys() { return systems.count(); }
    bool &oldSysEdit() { return _oldSysEdit; }
    QVector<int> &deletedSystems() { return deletedSys; }
    void addSystem(_bootloader, _typePlace, QString, QString, QString, bool);
    void delBackSystem() { systems.removeLast(); }
    void systemEnd() { systems.back().ended = true; }
    void read();
    void write();
    void execBars(QProgressBar*, QProgressBar*, QStatusBar*);

    void sysSetSuccess();
    void formatFlashDrive();
    bool isUsbAlreadyFormatted(QString);
    void saveFlashData(int = -1);
    bool isInvalidImage(
        #if WIN
            QString
        #endif
            );
    QPair<bool, QString> mountImage(QString);
    void unmountImage();
    void unpackSystem(sysImgExtractType);
    QString obsolutePath(QString);
    QStringList getDrives(QString);
    void formatPartExt4();
    QPair<bool, QString> getBcdEntry(QString, bool = true);
    QPair<bool, QString> findBcdId(QString, QString = QString());
    void registerBootloader(bool);
    void registerGrub4dos();
    bool installGrub4dos();
    void registerGrub2Uefi(bool, bool);
    bool installGrub2Uefi(bool);
    bool installGrub2BootmgrUefi(bool);
    void registerGrub2Bios();
    bool installGrub2Bios();
    bool registerGrub2Linux();
    bool installGrubUsb();
    void registerGrubUsb();
    QString grub2UsbConfigure(QString, bool = false, bool = true, int = -1);
    QString grub2Configure(QString, bool = false, bool = true, int = -1);
    QString grubLConfigure(QString, bool = false, bool = true);
    void createDataImg(int, bool);
    void downloadFile(QString, QString);
    void delSystemFiles(int);
    void deleteBootloaderEntry(int);
    void deleteGrubLEntry(int);
    void deleteGrub2Entry(int);
    void deleteGrub2UsbEntry(int);
    void deleteBootloader(int);
    void deleteGrubL(int);
    void deleteSettingsEntry(int);
};

#if WIN
#define checkAbortCmd() if(resCmd.first)  { emit abort(resCmd.second); returnFault(); }

#define checkAbortFile() if(!resFile) { emit abort(QObject::tr("Couldn't copy needed files")); returnFault(); }

#define COPY(src, dst) resFile = _copy(src, dst); checkAbortFile();

#define MKDIR(name) resFile = _mkdir(name); checkAbortFile();

#define RENAME(src, dst) resFile = _rename(src, dst); checkAbortFile();

#define REMOVE(name) resFile = _remove(name); checkAbortFile();

#define execAbort(command) resCmd = cmd::exec(command, true); checkAbortCmd();

#define logDirExist() qDebug().noquote() \
    << QString("%1 %2").arg(path, (res ? QObject::tr("exists") : QObject::tr("doesn't exist")));
#endif

#endif // INSTALLDATA_H

#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QSettings>
#include "3rdparty/enum.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

STRING_ENUM(_lang, en, ru)

class options : public QObject {
    Q_OBJECT
public:
    friend class Window;
    friend class install;
private:
    _lang lang;
    bool needSave = false;
    bool tbios;
    bool arch;
    const bool os = OS;
    bool winv = true;
    bool conEnable = false;
#if WIN
    QString efiGuid;
    QString efiMountPoint;
    bool efiWasMounted = false;
    bool efiMounted = false;
#endif
public slots:
    void write_set(bool, bool, bool, bool, bool, _lang);
public:
    bool getBios() const { return tbios; }
    bool getArch() const { return arch; }
    bool getConEnable() const { return conEnable; }
    void setConEnable(bool con) { conEnable = con; }
    _lang getLang() { return lang; }
    void setLang(_lang l) { lang = l; }
    void autowrite_set();
    bool read_set(bool);
    bool defbios();
    bool defarch();
#if WIN
    QString getEfiMountPoint() const { return efiMountPoint; }
    QPair<bool, QString> mountEfiPart();
    QPair<bool, QString> unmountEfiPart();
    QString freeMountPoint();
    bool defwinv();
#endif
};
#endif // DATA_H

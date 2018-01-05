#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QString>
#include <QSettings>
#include "enum.h"
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
public slots:
    void write_set(bool, bool, bool, bool, bool, _lang);
public:
    bool getConEnable() { return conEnable; }
    void setConEnable(bool con) { conEnable = con; }
    _lang getLang() { return lang; }
    void setLang(_lang l) { lang = l; }
    void autowrite_set();
    bool read_set(bool);
    bool defbios();
    bool defarch();
#if WIN
    bool defwinv();
#endif
};
#endif // DATA_H

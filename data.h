#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QString>
#include <QSettings>
#include "os.h"
#if OS == 1
#include <windows.h>
#endif


class options : public QObject {
    Q_OBJECT
public:
    friend class Window;
private:
    bool needSave = false;
    bool tbios;
    bool arch;
    const bool os = OS;
    bool winv = true;
public slots:
    void write_set(bool, bool, bool, bool);
public:
    void autowrite_set();
    bool read_set(bool);
    bool defbios();
    bool defarch();
#if OS == 1
    bool defwinv();
#endif
};
#endif // DATA_H

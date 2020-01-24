#ifndef GLOBAL_CPP
#define GLOBAL_CPP

#include <QString>
#include <QDir>
#include <QFile>
#include "options.h"
#include "install.h"
#include "version.h"

struct _global
{
    const QString VERSION = VER_PRODUCTVERSION_STR;
    options * const set;
    install * const insSet;
    _global(options *s, install *ins) : set(s), insSet(ins) {}

};
extern const _global *global;

auto _copy = [=](QString src, QString dst) -> bool
{
    qDebug().noquote() << QObject::tr("Copying %1 to %2").arg(src, dst);
    bool res = QFile(src).copy(dst);
    if(res) qDebug().noquote() << QObject::tr("Сopied successfully");
    else qDebug().noquote() << QObject::tr("Сopied unsuccessfully");
    return res;
};

auto _mkdir = [=](QString name) -> bool
{
    qDebug().noquote() << QObject::tr("Making dir %1").arg(name);
    bool res = QDir().mkdir(name);
    if(res) qDebug().noquote() << QObject::tr("Made successfully");
    else qDebug().noquote() << QObject::tr("Made unsuccessfully");
    return res;
};

auto _rename = [=](QString src, QString dst) -> bool
{
    qDebug().noquote() << QObject::tr("Renaming %1 to %2").arg(src, dst);
    bool res = QFile(src).rename(dst);
    if(res) qDebug().noquote() << QObject::tr("Renamed successfully");
    else qDebug().noquote() << QObject::tr("Renamed unsuccessfully");
    return res;
};

auto _remove = [=](QString name) -> bool
{
    qDebug().noquote() << QObject::tr("Deleting %1").arg(name);
    bool res = QFile::remove(name);
    if(res) qDebug().noquote() << QObject::tr("Deleted successfully");
    else qDebug().noquote() << QObject::tr("Deleted unsuccessfully");
    return res;
};

#endif // GLOBAL_CPP

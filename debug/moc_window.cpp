/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    QByteArrayData data[18];
    char stringdata0[417];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Window"
QT_MOC_LITERAL(1, 7, 16), // "returnMainWindow"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 16), // "Settings_clicked"
QT_MOC_LITERAL(4, 42, 25), // "on_applaySettings_clicked"
QT_MOC_LITERAL(5, 68, 26), // "on_restoreSettings_clicked"
QT_MOC_LITERAL(6, 95, 28), // "on_installButtonMain_clicked"
QT_MOC_LITERAL(7, 124, 26), // "on_buttonAboutMain_clicked"
QT_MOC_LITERAL(8, 151, 24), // "on_back_settings_clicked"
QT_MOC_LITERAL(9, 176, 28), // "on_buttonChooseImage_clicked"
QT_MOC_LITERAL(10, 205, 36), // "on_buttonChooseDirForInstall_..."
QT_MOC_LITERAL(11, 242, 31), // "on_buttonInstallInstall_clicked"
QT_MOC_LITERAL(12, 274, 32), // "on_comboBoot_currentIndexChanged"
QT_MOC_LITERAL(13, 307, 4), // "arg1"
QT_MOC_LITERAL(14, 312, 27), // "on_deleteButtonMain_clicked"
QT_MOC_LITERAL(15, 340, 40), // "on_comboSystemDelete_currentI..."
QT_MOC_LITERAL(16, 381, 5), // "index"
QT_MOC_LITERAL(17, 387, 29) // "on_buttonDeleteDelete_clicked"

    },
    "Window\0returnMainWindow\0\0Settings_clicked\0"
    "on_applaySettings_clicked\0"
    "on_restoreSettings_clicked\0"
    "on_installButtonMain_clicked\0"
    "on_buttonAboutMain_clicked\0"
    "on_back_settings_clicked\0"
    "on_buttonChooseImage_clicked\0"
    "on_buttonChooseDirForInstall_clicked\0"
    "on_buttonInstallInstall_clicked\0"
    "on_comboBoot_currentIndexChanged\0arg1\0"
    "on_deleteButtonMain_clicked\0"
    "on_comboSystemDelete_currentIndexChanged\0"
    "index\0on_buttonDeleteDelete_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x0a /* Public */,
       5,    0,   87,    2, 0x0a /* Public */,
       6,    0,   88,    2, 0x0a /* Public */,
       7,    0,   89,    2, 0x0a /* Public */,
       8,    0,   90,    2, 0x0a /* Public */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    1,   94,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    1,   98,    2, 0x08 /* Private */,
      17,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Window *_t = static_cast<Window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->returnMainWindow(); break;
        case 1: _t->Settings_clicked(); break;
        case 2: _t->on_applaySettings_clicked(); break;
        case 3: _t->on_restoreSettings_clicked(); break;
        case 4: _t->on_installButtonMain_clicked(); break;
        case 5: _t->on_buttonAboutMain_clicked(); break;
        case 6: _t->on_back_settings_clicked(); break;
        case 7: _t->on_buttonChooseImage_clicked(); break;
        case 8: _t->on_buttonChooseDirForInstall_clicked(); break;
        case 9: _t->on_buttonInstallInstall_clicked(); break;
        case 10: _t->on_comboBoot_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->on_deleteButtonMain_clicked(); break;
        case 12: _t->on_comboSystemDelete_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_buttonDeleteDelete_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Window.data,
      qt_meta_data_Window,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(const_cast< Window*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

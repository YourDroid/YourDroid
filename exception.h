//#ifndef QT_BREAKPAD_H
//#define QT_BREAKPAD_H

//#include <QCoreApplication>
//#include <QDir>
//#include <QFile>
//#include <QProcess>
//#include <QString>

//#if defined(Q_OS_WIN32)
//// breakpad includes windows.h, so we must include QT's qt_windows.h beforehand
//// to avoid breaking other QT headers
//#include <qt_windows.h>

//#include "client/windows/handler/exception_handler.h"

//#elif defined(Q_OS_LINUX)
//#include "client/linux/handler/exception_handler.h"

//#endif // OS-specific includes

//class QtBreakpad
//{
//public:
//    static void init(const QString& reportPath,
//                     google_breakpad::ExceptionHandler::FilterCallback filterCallBack = NULL,
//                     google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback = NULL,
//                     void* callbackContext = NULL);

//    typedef bool (*QMinidumpCallback)(QFile& minidumpFile, void* context);

//    static void init(const QString& reportPath,
//                     QMinidumpCallback qMinidumpCallback,
//                     void* callbackContext = NULL);

//private:
//    Q_DISABLE_COPY(QtBreakpad)
//    static QtBreakpad* _instance;
//    static void replaceInstance(QtBreakpad* newInstance);

//    class QMinidumpContextWrapper
//    {
//        public:
//            QMinidumpContextWrapper(QMinidumpCallback qMinicudmpCallback, void* context)
//            {
//                this->qMinicudmpCallback = qMinicudmpCallback;
//                this->context = context;
//            }

//            ~QMinidumpContextWrapper() { }

//            QMinidumpCallback qMinicudmpCallback;
//            void* context;
//    };

//    QtBreakpad(const QString& reportPath,
//               google_breakpad::ExceptionHandler::FilterCallback filterCallBack,
//               google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback,
//               void* callbackContext);
//    QtBreakpad(const QString& reportPath,
//               QMinidumpCallback qMinidumpCallback,
//               void* callbackContext);
//    ~QtBreakpad();

//    google_breakpad::ExceptionHandler* _breakpad_handler;
//    void buildBreakpadHandler(const QString& reportPath,
//                              google_breakpad::ExceptionHandler::FilterCallback filterCallBack,
//                              google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback,
//                              void* callbackContext);

//    QMinidumpContextWrapper* _qMinidumpContextWrapper;
//#if defined(Q_OS_WIN32)
//    static bool qMinidumpWrapper(const wchar_t* dump_path,
//                                 const wchar_t* minidump_id,
//                                 QMinidumpContextWrapper* context,
//                                 EXCEPTION_POINTERS* exinfo,
//                                 MDRawAssertionInfo* assertion,
//                                 bool succeeded);
//#elif defined(Q_OS_LINUX)
//    static bool qMinidumpWrapper(const MinidumpDescriptor& descriptor,
//                                 QMinidumpContextWrapper* context,
//                                 bool succeeded);
//#endif
//};

//#endif // QT_BREAKPAD_H

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdlib.h>
#include <signal.h>
#include <QString>

#if LINUX
#include <err.h>
#endif

#if WIN
#include <exception>
#include <VirtDisk.h>
#include <windows.h>
#elif LINUX
#include <err.h>
#endif

namespace Breakpad {
    class CrashHandlerPrivate;
    class CrashHandler
    {
    public:
        static CrashHandler* instance();
    void Init(const QString&  reportPath);

        void setReportCrashesToSystem(bool report);
        bool writeMinidump();

    private:
        CrashHandler();
        ~CrashHandler();
        Q_DISABLE_COPY(CrashHandler)
        CrashHandlerPrivate* d;
    };
}

#if LINUX
void gdb_SetProgName (char *prog_name);
#endif

void printStack();

void errorAbort(int);

#if WIN
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS*);
#elif LINUX
void posix_signal_handler(int, siginfo_t*, void*);
#endif

void all_handler(int);

void set_signal_handler();

#endif // EXCEPTION_H

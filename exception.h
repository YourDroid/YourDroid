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

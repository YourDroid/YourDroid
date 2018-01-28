#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdlib.h>
#include <signal.h>

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

void printStack();

void errorAbort(int);

#if WIN
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS*);
#elif LINUX
void posix_signal_handler(int, siginfo_t*, void*);
#endif

//void all_handler(int);

void set_signal_handler();

#endif // EXCEPTION_H

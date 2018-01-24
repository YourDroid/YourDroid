#include "data.h"
#include "window.h"
#include "install.h"
#include <version.h>
#include "log.h"
#include "cmd.h"
#include "console.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

#if WIN
#include <exception>
#include <VirtDisk.h>
#include <windows.h>
#elif LINUX
#include <err.h>
#endif

const QString VERSION = VER_PRODUCTVERSION_STR;
static QString workDir;
const QString &WORK_DIR = workDir;
console *log::con;

#if WIN
LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
{
    QString type, description;
    switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        type = "EXCEPTION_ACCESS_VIOLATION";
        description = QObject::tr("The thread tried to read from or write to a virtual address for "
                      "which it does not have the appropriate access.");
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        type = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        description = QObject::tr("The thread tried to access an array element that is out of bounds and "
                      "the underlying hardware supports bounds checking.");
        break;
    case EXCEPTION_BREAKPOINT:
        type = "EXCEPTION_BREAKPOINT";
        description = QObject::tr("A breakpoint was encountered.");
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        type = "EXCEPTION_DATATYPE_MISALIGNMENT";
        description = QObject::tr("The thread tried to read or write data that is misaligned on "
                      "hardware that does not provide alignment. "
                      "For example, 16-bit values must be aligned on 2-byte boundaries; "
                      "32-bit values on 4-byte boundaries, and so on.");
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        type = "EXCEPTION_FLT_DENORMAL_OPERAND";
        description = QObject::tr("One of the operands in a floating-point operation is denormal. "
                      "A denormal value is one that is too small to represent as a standard floating-point value.");
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        type = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
        description = QObject::tr("The thread tried to divide a floating-point value by a floating-point divisor of zero.");
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        type = "EXCEPTION_FLT_INEXACT_RESULT";
        description = QObject::tr("The result of a floating-point operation cannot be represented exactly "
                                  "as a decimal fraction.");
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        type = "EXCEPTION_FLT_INVALID_OPERATION";
        description = QObject::tr("This exception represents any floating-point exception not included in this list.");
        break;
    case EXCEPTION_FLT_OVERFLOW:
        type = "EXCEPTION_FLT_OVERFLOW";
        description = QObject::tr("The exponent of a floating-point operation is greater than "
                                  "the magnitude allowed by the corresponding type.");
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        type = "EXCEPTION_FLT_STACK_CHECK";
        description = QObject::tr("The stack overflowed or underflowed as the result of a floating-point operation.");
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        type = "EXCEPTION_FLT_UNDERFLOW";
        description = QObject::tr("The exponent of a floating-point operation is less than "
                                  "the magnitude allowed by the corresponding type.");
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        type = "EXCEPTION_ILLEGAL_INSTRUCTION";
        description = QObject::tr("The thread tried to execute an invalid instruction.");
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        type = "EXCEPTION_IN_PAGE_ERROR";
        description = QObject::tr("The thread tried to access a page that was not present, and "
                                  "the system was unable to load the page. For example, "
                                  "this exception might occur if a network connection is lost while "
                                  "running a program over the network.");
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        type = "EXCEPTION_INT_DIVIDE_BY_ZERO";
        description = QObject::tr("The thread tried to divide an integer value by an integer divisor of zero.");
        break;
    case EXCEPTION_INT_OVERFLOW:
        type = "EXCEPTION_INT_OVERFLOW";
        description = QObject::tr("The result of an integer operation caused a carry out of "
                                  "the most significant bit of the result.");
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        type = "EXCEPTION_INVALID_DISPOSITION";
        description = QObject::tr("An exception handler returned an invalid disposition to the exception dispatcher. "
                                  "Programmers using a high-level language such as C should never encounter this exception.");
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        type = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
        description = QObject::tr("The thread tried to continue execution after a noncontinuable exception occurred.");
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        type = "EXCEPTION_PRIV_INSTRUCTION";
        description = QObject::tr("The thread tried to execute an instruction whose "
                                  "operation is not allowed in the current machine mode.");
        break;
    case EXCEPTION_SINGLE_STEP:
        type = "EXCEPTION_SINGLE_STEP";
        description = QObject::tr("A trace trap or other single-instruction mechanism signaled that "
                                  "one instruction has been executed.");
        break;
    case EXCEPTION_STACK_OVERFLOW:
        type = "EXCEPTION_STACK_OVERFLOW";
        description = QObject::tr("The thread used up its stack.");
        break;
    default:
        type = QObject::tr("Unrecognized Exception");
        description = QObject::tr("No description");
        break;
    }
    unsigned long code = ExceptionInfo->ExceptionRecord->ExceptionCode;
    qCritical().noquote() <<
                             QObject::tr("^Fatal error! Here is some info about it: description: %1; type: %2; code: %3")
                             .arg(description).arg(type).arg(code);
    exit(3);
    return EXCEPTION_EXECUTE_HANDLER;
}
#elif LINUX
void posix_signal_handler(int sig, siginfo_t *siginfo, void *context)
{
    switch(sig)
      {
        case SIGSEGV:
          type = "SIGSEGV"; description = "Segmentation Fault";
          break;
        case SIGINT:
          type = "SIGINT"; description = "Interactive attention signal, (usually ctrl+c)\n",
                stderr);
          break;
        case SIGFPE:
          switch(siginfo->si_code)
          {
            case FPE_INTDIV:
              type = "SIGFPE"; description = "integer divide by zero";
              break;
            case FPE_INTOVF:
              type = "SIGFPE"; description = "integer overflow";
              break;
            case FPE_FLTDIV:
              type = "SIGFPE"; description = "floating-point divide by zero";
              break;
            case FPE_FLTOVF:
              type = "SIGFPE"; description = "floating-point overflow";
              break;
            case FPE_FLTUND:
              type = "SIGFPE"; description = "floating-point underflow";
              break;
            case FPE_FLTRES:
              type = "SIGFPE"; description = "floating-point inexact result";
              break;
            case FPE_FLTINV:
              type = "SIGFPE"; description = "floating-point invalid operation";
              break;
            case FPE_FLTSUB:
              type = "SIGFPE"; description = "subscript out of range";
              break;
            default:
              type = "SIGFPE"; description = "Arithmetic Exception";
              break;
          }
        case SIGILL:
          switch(siginfo->si_code)
          {
            case ILL_ILLOPC:
              type = "SIGILL"; description = "illegal opcode";
              break;
            case ILL_ILLOPN:
              type = "SIGILL"; description = "illegal operand";
              break;
            case ILL_ILLADR:
              type = "SIGILL"; description = "illegal addressing mode";
              break;
            case ILL_ILLTRP:
              type = "SIGILL"; description = "illegal trap";
              break;
            case ILL_PRVOPC:
              type = "SIGILL"; description = "privileged opcode";
              break;
            case ILL_PRVREG:
              type = "SIGILL"; description = "privileged register";
              break;
            case ILL_COPROC:
              type = "SIGILL"; description = "coprocessor error";
              break;
            case ILL_BADSTK:
              type = "SIGILL"; description = "internal stack error";
              break;
            default:
              type = "SIGILL"; description = "Illegal Instruction";
              break;
          }
          break;
        case SIGTERM:
          type = "SIGTERM"; description = "a termination request was sent to the program\n",
                stderr);
          break;
        case SIGABRT:
          type = "SIGABRT"; description = "usually caused by an abort() or assert()";
          break;
        default:
          break;
      }
    qCritical().noquote() <<
                             QObject::tr("^Fatal error! Here is some info about it: description = %1"
                                         "exception code: %2; exception info code %3")
                             .arg(description).arg(sig, siginfo->si_code);
    exit(3);
}
#endif

void all_handler(int sig)
{
    QString type, description;
    switch(sig)
    {
    case SIGABRT:
        type = "SIGABRT"; description = "usually caused by an abort() or assert()";
        break;
    case SIGFPE:
        type = "SIGFPE"; description = "arithmetic exception, such as divide by zero";
        break;
    case SIGILL:
        type = "SIGILL"; description = "illegal instruction";
        break;
    case SIGINT:
        type = "SIGINT"; description = "interactive attention signal, probably a ctrl+c";
        break;
    case SIGSEGV:
        type = "SIGSEGV"; description = "segmentation fault";
        break;
    case SIGTERM:
    default:
        type = "SIGTERM"; description = "a termination request was sent to the program";
        break;
    }
    qCritical().noquote() << QObject::tr("^Fatal error: description: %1; type: %2; code: %3")
                   .arg(description, type, QString::number(sig));
    signal(sig, SIG_DFL);
    exit(1);
}


void set_signal_handler()
{
#if WIN
    SetUnhandledExceptionFilter(windows_exception_handler);
#elif LINUX
    {
        struct sigaction sig_action = {};
        sig_action.sa_sigaction = posix_signal_handler;
        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

        if (sigaction(SIGSEGV, &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGFPE,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGINT,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGILL,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGTERM, &sig_action, NULL) != 0) { err(1, "sigaction"); }
        if (sigaction(SIGABRT, &sig_action, NULL) != 0) { err(1, "sigaction"); }
    }
#endif
    signal(SIGABRT, all_handler);
    signal(SIGFPE,  all_handler);
    signal(SIGILL,  all_handler);
    signal(SIGINT,  all_handler);
    signal(SIGSEGV, all_handler);
    signal(SIGTERM, all_handler);
}

int main(int argc, char *argv[])
{
    std::freopen("./log/stderr.txt", "a+", stderr);
    fprintf(stderr, "\n\n###NEW###");
    set_signal_handler();
    std::set_terminate([=](){
        qCritical().noquote() << QObject::tr("^Unknown fatal error! Program will terminate");
        exit(1);
    });

    try {
        QApplication app(argc,argv);
        qRegisterMetaType<QtMsgType>("QtMsgType");
        workDir = app.applicationDirPath();
        qInstallMessageHandler(log::messagenew);
#if LINUX
        if(!QFile().exists(QString::fromLocal8Bit(qgetenv("HOME")) + "/.config/QtProject/qtlogging.ini"))
            system("touch ~/.config/QtProject/qtlogging.ini");
#endif
        console *widget = log::init();
#if LINUX
        int uid = geteuid();
        qDebug().noquote() << QObject::tr("getuid() returned %1").arg(uid);
        if(uid != 0) {
            if(!QProcess::startDetached(QString("gksudo %1").arg(qApp->arguments()[0]))) {
                qCritical() << QObject::tr("^Program must be run with root. Run \'sudo %1\' in the terminal to fix it")
                               .arg(qApp->applicationFilePath());
                return 1;
            }
            return 0;
        }
#endif
        options set;
        bool readSet = set.read_set(false);

        QTranslator translator;
        translator.load("yourdroid_" + QString::fromStdString(_langHelper::to_string(set.getLang())));
        app.installTranslator(&translator);
        if(argc == 2 && argv[1] == "c" || set.getConEnable()) log::setEnabledCon(true);

        cmd::exec("help");
        int *i;
        i = 0;
        *i = 9;

        qDebug().noquote() << QString(app.translate("log", "Work dir is ")) + WORK_DIR;
        install ins(&set);
        ins.read();

        Window *window = new Window(&ins, readSet, &set);
        window->show();
        QObject::connect(window, &Window::closed, [=](){ widget->close(); });
        QObject::connect(widget, &console::hided, [=](){ window->consoleHided(); });

        qDebug().noquote() << app.translate("log", "Window exec");
        int res = app.exec();
        qDebug().noquote() << app.translate("log", "Window closed");
        set.autowrite_set();
        ins.write();
        qDebug().noquote() << QObject::tr("Exiting... Returned ") << QString::number(res);
        return res;
    }
    catch(std::exception ex) {
        qCritical().noquote() << QObject::tr("^Fatal error: %1").arg(QString::fromStdString(ex.what()));
        return 1;
    }
    catch(...) {
        qCritical().noquote() << QObject::tr("^Unknown fatal error!");
        return 1;
    }
}

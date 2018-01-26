#include "exception.h"
#include "window.h"

#if LINUX
#include <err.h>
#include <execinfo.h>
#endif


void errorAbort(int ret) {
#if LINUX
    void * array[25];
    int nSize = backtrace(array, 25);
    char **_back = backtrace_symbols(array, nSize);
    QString back;
    for(int i = 0; i < nSize; i++) back += QString(_back[i]) + '\n';
    qDebug().noquote() << QString("Backtrace is: %1").arg(back);
#endif
    exit(ret);
}

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
    qCritical().noquote().noquote() <<
                                       QObject::tr("^Fatal error! Here is some info about it: description: %1; type: %2; code: %3")
                                       .arg(description).arg(type).arg(code);
    errorAbort(1);
    return EXCEPTION_EXECUTE_HANDLER;
}
#elif LINUX
void posix_signal_handler(int sig, siginfo_t *siginfo, void *context)
{
    QString type, description;
    switch(sig)
    {
    case SIGSEGV:
        type = "SIGSEGV"; description = "Segmentation Fault";
        break;
    case SIGINT:
        type = "SIGINT"; description = "Interactive attention signal, (usually ctrl+c)";
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
        type = "SIGTERM"; description = "a termination request was sent to the program";
        break;
    case SIGABRT:
        type = "SIGABRT"; description = "usually caused by an abort() or assert()";
        break;
    default:
        break;
    }
    qCritical().noquote().noquote() <<
                                       QObject::tr("^Fatal error! Here is some info about it: description: %1; "
                                                   "exception code: %2; exception info code %3")
                                       .arg(description).arg(sig).arg(siginfo->si_code);
    errorAbort(1);
}
#endif

//void all_handler(int sig)
//{
//    QString type, description;
//    switch(sig)
//    {
//    case SIGABRT:
//        type = "SIGABRT"; description = "usually caused by an abort() or assert()";
//        break;
//    case SIGFPE:
//        type = "SIGFPE"; description = "arithmetic exception, such as divide by zero";
//        break;
//    case SIGILL:
//        type = "SIGILL"; description = "illegal instruction";
//        break;
//    case SIGINT:
//        type = "SIGINT"; description = "interactive attention signal, probably a ctrl+c";
//        break;
//    case SIGSEGV:
//        type = "SIGSEGV"; description = "segmentation fault";
//        break;
//    case SIGTERM:
//    default:
//        type = "SIGTERM"; description = "a termination request was sent to the program";
//        break;
//    }
//    qCritical().noquote().noquote() << QObject::tr("^Fatal error: description: %1; type: %2; code: %3")
//                   .arg(description, type, QString::number(sig));
//    signal(sig, SIG_DFL);
//    errorAbort(1);
//}


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
    //    signal(SIGABRT, all_handler);
    //    signal(SIGFPE,  all_handler);
    //    signal(SIGILL,  all_handler);
    //    signal(SIGINT,  all_handler);
    //    signal(SIGSEGV, all_handler);
    //    signal(SIGTERM, all_handler);
}

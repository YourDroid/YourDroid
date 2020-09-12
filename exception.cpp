////#include "exception.h"

////QtBreakpad* QtBreakpad::_instance = NULL;

////void QtBreakpad::init(const QString& reportPath,
////                      google_breakpad::ExceptionHandler::FilterCallback filterCallBack,
////                      google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback,
////                      void* callbackContext)
////{
////    QtBreakpad::replaceInstance(new QtBreakpad(reportPath, filterCallBack,
////                                               minidumpCallback, callbackContext));
////}

////void QtBreakpad::init(const QString& reportPath,
////                      QMinidumpCallback qMinidumpCallback,
////                      void* callbackContext)
////{
////    QtBreakpad::replaceInstance(new QtBreakpad(reportPath, qMinidumpCallback, callbackContext));
////}

////QtBreakpad::QtBreakpad(const QString& reportPath,
////                       google_breakpad::ExceptionHandler::FilterCallback filterCallBack,
////                       google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback,
////                       void* callbackContext)
////{
////    this->buildBreakpadHandler(reportPath, filterCallBack, minidumpCallback, callbackContext);
////}

////void QtBreakpad::buildBreakpadHandler(const QString& reportPath,
////                                      google_breakpad::ExceptionHandler::FilterCallback filterCallBack,
////                                      google_breakpad::ExceptionHandler::MinidumpCallback minidumpCallback,
////                                      void* callbackContext)
////{
////#if defined(Q_OS_WIN32)
////    std::wstring pathAsStr = (const wchar_t*)reportPath.utf16();
////    this->_breakpad_handler = new google_breakpad::ExceptionHandler(
////        pathAsStr,
////        filterCallBack,
////        minidumpCallback,
////        callbackContext,
////        true
////    );
////#elif defined(Q_OS_LINUX)
////    std::string pathAsStr = reportPath.toStdString();
////    google_breakpad::MinidumpDescriptor md(reportPath);
////    this->_breakpad_handler = new google_breakpad::ExceptionHandler(
////        md,
////        filterCallBack,
////        minidumpCallback,
////        callbackContext,
////        true,
////        -1
////    );
////#endif
////}

////QtBreakpad::QtBreakpad(const QString& reportPath,
////           QMinidumpCallback qMinidumpCallback,
////           void* callbackContext)
////{
////    this->_qMinidumpContextWrapper = new QtBreakpad::QMinidumpContextWrapper(qMinidumpCallback, callbackContext);

////    this->buildBreakpadHandler(reportPath,
////                               NULL, // filter callback
////                               reinterpret_cast<google_breakpad::ExceptionHandler::MinidumpCallback>(&QtBreakpad::qMinidumpWrapper),
////                               this->_qMinidumpContextWrapper);
////}

////#if defined(Q_OS_WIN32)
////bool QtBreakpad::qMinidumpWrapper(const wchar_t* dump_path,
////                                  const wchar_t* minidump_id,
////                                  QMinidumpContextWrapper* contextWrapper,
////                                  EXCEPTION_POINTERS* exinfo,
////                                  MDRawAssertionInfo* assertion,
////                                  bool succeeded)
////{
////    Q_UNUSED(exinfo);
////    Q_UNUSED(assertion);
////#elif defined(Q_OS_LINUX)
////bool QtBreakpad::qMinidumpWrapper(const MinidumpDescriptor& descriptor,
////                                  QMinidumpContextWrapper* contextWrapper,
////                                  bool succeeded)
////{
////#endif
////    // if there's no dump file, not much to do
////    if (!succeeded) { return false; }

////    QMinidumpCallback qMinicudmpCallback = contextWrapper->qMinicudmpCallback;
////    void* context = contextWrapper->context;

////    QString minidumpFileName;

////#if defined(Q_OS_WIN32)
////    QDir minidumpDir = QDir(QString::fromWCharArray(dump_path));
////    minidumpFileName = minidumpDir.absoluteFilePath(QString::fromWCharArray(minidump_id) + ".dmp");
////#elif defined(Q_OS_LINUX)
////    minidumpFileName = descriptor.path();
////#endif

////    QFile minidumpFile(minidumpFileName);

////    return qMinicudmpCallback(minidumpFile, context);
////}

////void QtBreakpad::replaceInstance(QtBreakpad* newInstance)
////{
////    // there can only be one handler
////    delete QtBreakpad::_instance;

////    QtBreakpad::_instance = newInstance;
////}

////QtBreakpad::~QtBreakpad()
////{
////    delete this->_breakpad_handler;
////    delete this->_qMinidumpContextWrapper;
////}


//#include "exception.h"
//#include "window.h"
//#include <cmd.h>


//#if LINUX
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <err.h>
//#include <execinfo.h>
//#elif WIN
//#include <DbgHelp.h>
//#endif

//#include <QDir>
//#include <QProcess>
//#include <QCoreApplication>
//#include <QString>

//#if LINUX
//#define SYS_ERROR(msg) \
//    qWarning().noquote() << QObject::tr("*** stack print error *** at %1 %2: %3 (%4)") \
//    .arg(__FILE__, __LINE__).arg(msg).arg(strerror (errno));

//#define SYS_ASSERT(cond, err_msg) \
//    do \
//{ \
//    if (cond) \
//    ; \
//    else \
//{ \
//    SYS_ERROR (err_msg); \
//    return; \
//    } \
//    } while (0)

//static char *gdb_ProgramName;

//void gdb_SetProgName (char *prog_name)
//{
//    gdb_ProgramName = prog_name;
//}
//#endif

//void printStack()
//{
//#if WIN
//    HANDLE process = GetCurrentProcess();
//    HANDLE thread = GetCurrentThread();

//    CONTEXT context;
//    memset(&context, 0, sizeof(CONTEXT));
//    context.ContextFlags = CONTEXT_FULL;
//    RtlCaptureContext(&context);

//    SymInitialize(process, NULL, TRUE);

//    DWORD image;
//    STACKFRAME64 stackframe;
//    ZeroMemory(&stackframe, sizeof(STACKFRAME64));

//#ifdef _M_IX86
//    image = IMAGE_FILE_MACHINE_I386;
//    stackframe.AddrPC.Offset = context.Eip;
//    stackframe.AddrPC.Mode = AddrModeFlat;
//    stackframe.AddrFrame.Offset = context.Ebp;
//    stackframe.AddrFrame.Mode = AddrModeFlat;
//    stackframe.AddrStack.Offset = context.Esp;
//    stackframe.AddrStack.Mode = AddrModeFlat;
//#elif _M_X64
//    image = IMAGE_FILE_MACHINE_AMD64;
//    stackframe.AddrPC.Offset = context.Rip;
//    stackframe.AddrPC.Mode = AddrModeFlat;
//    stackframe.AddrFrame.Offset = context.Rsp;
//    stackframe.AddrFrame.Mode = AddrModeFlat;
//    stackframe.AddrStack.Offset = context.Rsp;
//    stackframe.AddrStack.Mode = AddrModeFlat;
//#elif _M_IA64
//    image = IMAGE_FILE_MACHINE_IA64;
//    stackframe.AddrPC.Offset = context.StIIP;
//    stackframe.AddrPC.Mode = AddrModeFlat;
//    stackframe.AddrFrame.Offset = context.IntSp;
//    stackframe.AddrFrame.Mode = AddrModeFlat;
//    stackframe.AddrBStore.Offset = context.RsBSP;
//    stackframe.AddrBStore.Mode = AddrModeFlat;
//    stackframe.AddrStack.Offset = context.IntSp;
//    stackframe.AddrStack.Mode = AddrModeFlat;
//#endif

//    QString trace;
//    for (size_t i = 0; i < 25; i++) {

//        BOOL result = StackWalk64(
//                    image, process, thread,
//                    &stackframe, &context, NULL,
//                    SymFunctionTableAccess64, SymGetModuleBase64, NULL);

//        if (!result) { break; }

//        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
//        PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
//        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
//        symbol->MaxNameLen = MAX_SYM_NAME;

//        DWORD64 displacement = 0;
//        if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol)) {
//            trace += QString("[%1] %2\n").arg(i).arg(symbol->Name);
//        } else {
//            trace += QString("[%1] ???\n").arg(i);
//        }

//    }
//    qDebug().noquote() << QObject::tr("Backtrace is:\n%1").arg(trace);

//    SymCleanup(process);
//#elif LINUX
//    //cmd::exec()
//    char buff[L_tmpnam];
//    const char *scr_file_name;
//    FILE *scr_file;
//    pid_t child_pid;

//    /* Создаем имя временного файла. Использовать "правильные" интерфейсы
//   * типа tempnam или tmpfile не получится, потому как имя файла нам надо
//   * передать в gdb, а с этими интерфейсами имя файла не доступно.
//   * При линковке программы под linux'ом мы увидим предупреждение что-то типа
//   * "warning: the use of `tmpnam' is dangerous, better use `mkstemp'" */
//    scr_file_name = tmpnam (buff);
//    SYS_ASSERT (scr_file_name != NULL, "failed to gen tmp file name");

//    /* В файле формируем приказы для gdb. Этот файл будет передан по опции
//   * в отладчик. В нешем случае нам нужно напечатать стек (bt) и выйти (q) */
//    scr_file = fopen (scr_file_name, "w");
//    SYS_ASSERT (scr_file, "child: failed access gdb script");
//    fprintf (scr_file, "bt\nq\n");
//    fclose (scr_file);

//    /* Форкаемся для запуска отладчика в дочернем процессе и ожижания
//   * в родительском. Перед fork'ом можно сделать создать pipe, в дочернем
//   * процессе через dup2 замкнуть не него stdout и stderr, а в родительском
//   * процессе прочитать оттуда текст (выдачу отладчика) с тем, чтобы
//   * отфильтровать ненужные строки. Я этого делать не стану, дабы не усложнять
//   * программу. Тем более, что в разных версиях отладчика печать может выглядеть
//   * по разному */
//    child_pid = fork();
//    SYS_ASSERT (child_pid >= 0, "fork failed");

//    if (child_pid == 0)
//    {
//        char pid_str[32];

//        /* Дочерний процесс: запускаем gdb
//       * Отладчику передаём pid процесса, к которому он должен прицепиться.
//       * Поскольку мы после fork'а находимся в дочернем процессе, то для нас
//       * это означает pid родительского процесса */
//        sprintf (pid_str, "%d", (int)getppid());

//        /* Исходим из предположения, что gdb находится в путях, прописанных
//       * в PATH. Если у кого-то это не так, то вместо "gdb" надо будет прописать
//       * полный путь до отладчика */
//        execlp ("gdb", "gdb", gdb_ProgramName, pid_str, "-q", "--batch", "-x", scr_file_name, NULL);

//        /* В случае успешного запска сюда не попадём */
//        SYS_ASSERT (0, "child: failed to exec gdb");
//    }
//    else
//    {
//        pid_t exited_pid;
//        int child_status;

//        /* Родительский процесс: ожидаем завершение работы дочернего процесса
//       * (из-под которого запускается gdb) */
//        exited_pid = wait (&child_status);
//        SYS_ASSERT (exited_pid == child_pid, "parent: error waiting child to die");
//        SYS_ASSERT (WIFEXITED (child_status) && WEXITSTATUS (child_status) == 0,
//                    "parent: abnormal child termination");
//    }

//    /* Временный файл надо удалятьручками */
//    unlink (scr_file_name);
//#endif
//}

//void errorAbort(int ret) {
//#if LINUX
//    cmd::exec(QString("gcore %1 -o %2").arg(getpid()).arg(qApp->applicationDirPath() + QString("/log/dump-") +
//                                                          QDate::currentDate().toString("dMyy") +
//                                                          QTime::currentTime().toString("hhmmss")));
//    return;
//    if(cmd::exec("dpkg -s gdb").first) {
//        void * array[25];
//        int nSize = backtrace(array, 25);
//        char **_back = backtrace_symbols(array, nSize);
//        QString back;
//        for(int i = 0; i < nSize; i++) back += QString(_back[i]) + '\n';
//        qDebug().noquote() << QString("Backtrace is:\n%1").arg(back);
//    }
//    else
//#endif
//        printStack();
//#if WIN
//    QProcess::startDetached(QString("taskkill /pid %1").arg(getpid()));
//#endif
//    exit(ret);
//}

//#if WIN
//LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS *ExceptionInfo)
//{
//    QString type, description;
//    switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
//    {
//    case EXCEPTION_ACCESS_VIOLATION:
//        type = "EXCEPTION_ACCESS_VIOLATION";
//        description = QObject::tr("The thread tried to read from or write to a virtual address for "
//                                  "which it does not have the appropriate access.");
//        break;
//    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
//        type = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
//        description = QObject::tr("The thread tried to access an array element that is out of bounds and "
//                                  "the underlying hardware supports bounds checking.");
//        break;
//    case EXCEPTION_BREAKPOINT:
//        type = "EXCEPTION_BREAKPOINT";
//        description = QObject::tr("A breakpoint was encountered.");
//        break;
//    case EXCEPTION_DATATYPE_MISALIGNMENT:
//        type = "EXCEPTION_DATATYPE_MISALIGNMENT";
//        description = QObject::tr("The thread tried to read or write data that is misaligned on "
//                                  "hardware that does not provide alignment. "
//                                  "For example, 16-bit values must be aligned on 2-byte boundaries; "
//                                  "32-bit values on 4-byte boundaries, and so on.");
//        break;
//    case EXCEPTION_FLT_DENORMAL_OPERAND:
//        type = "EXCEPTION_FLT_DENORMAL_OPERAND";
//        description = QObject::tr("One of the operands in a floating-point operation is denormal. "
//                                  "A denormal value is one that is too small to represent as a standard floating-point value.");
//        break;
//    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
//        type = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
//        description = QObject::tr("The thread tried to divide a floating-point value by a floating-point divisor of zero.");
//        break;
//    case EXCEPTION_FLT_INEXACT_RESULT:
//        type = "EXCEPTION_FLT_INEXACT_RESULT";
//        description = QObject::tr("The result of a floating-point operation cannot be represented exactly "
//                                  "as a decimal fraction.");
//        break;
//    case EXCEPTION_FLT_INVALID_OPERATION:
//        type = "EXCEPTION_FLT_INVALID_OPERATION";
//        description = QObject::tr("This exception represents any floating-point exception not included in this list.");
//        break;
//    case EXCEPTION_FLT_OVERFLOW:
//        type = "EXCEPTION_FLT_OVERFLOW";
//        description = QObject::tr("The exponent of a floating-point operation is greater than "
//                                  "the magnitude allowed by the corresponding type.");
//        break;
//    case EXCEPTION_FLT_STACK_CHECK:
//        type = "EXCEPTION_FLT_STACK_CHECK";
//        description = QObject::tr("The stack overflowed or underflowed as the result of a floating-point operation.");
//        break;
//    case EXCEPTION_FLT_UNDERFLOW:
//        type = "EXCEPTION_FLT_UNDERFLOW";
//        description = QObject::tr("The exponent of a floating-point operation is less than "
//                                  "the magnitude allowed by the corresponding type.");
//        break;
//    case EXCEPTION_ILLEGAL_INSTRUCTION:
//        type = "EXCEPTION_ILLEGAL_INSTRUCTION";
//        description = QObject::tr("The thread tried to execute an invalid instruction.");
//        break;
//    case EXCEPTION_IN_PAGE_ERROR:
//        type = "EXCEPTION_IN_PAGE_ERROR";
//        description = QObject::tr("The thread tried to access a page that was not present, and "
//                                  "the system was unable to load the page. For example, "
//                                  "this exception might occur if a network connection is lost while "
//                                  "running a program over the network.");
//        break;
//    case EXCEPTION_INT_DIVIDE_BY_ZERO:
//        type = "EXCEPTION_INT_DIVIDE_BY_ZERO";
//        description = QObject::tr("The thread tried to divide an integer value by an integer divisor of zero.");
//        break;
//    case EXCEPTION_INT_OVERFLOW:
//        type = "EXCEPTION_INT_OVERFLOW";
//        description = QObject::tr("The result of an integer operation caused a carry out of "
//                                  "the most significant bit of the result.");
//        break;
//    case EXCEPTION_INVALID_DISPOSITION:
//        type = "EXCEPTION_INVALID_DISPOSITION";
//        description = QObject::tr("An exception handler returned an invalid disposition to the exception dispatcher. "
//                                  "Programmers using a high-level language such as C should never encounter this exception.");
//        break;
//    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
//        type = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
//        description = QObject::tr("The thread tried to continue execution after a noncontinuable exception occurred.");
//        break;
//    case EXCEPTION_PRIV_INSTRUCTION:
//        type = "EXCEPTION_PRIV_INSTRUCTION";
//        description = QObject::tr("The thread tried to execute an instruction whose "
//                                  "operation is not allowed in the current machine mode.");
//        break;
//    case EXCEPTION_SINGLE_STEP:
//        type = "EXCEPTION_SINGLE_STEP";
//        description = QObject::tr("A trace trap or other single-instruction mechanism signaled that "
//                                  "one instruction has been executed.");
//        break;
//    case EXCEPTION_STACK_OVERFLOW:
//        type = "EXCEPTION_STACK_OVERFLOW";
//        description = QObject::tr("The thread used up its stack.");
//        break;
//    default:
//        type = QObject::tr("Unrecognized Exception");
//        description = QObject::tr("No description");
//        break;
//    }
//    HANDLE hFile;

//    hFile = CreateFile(
//                (qApp->applicationDirPath() + QString("/log/dump-") +
//                 QDate::currentDate().toString("dMyy") +
//                 QTime::currentTime().toString("hhmmss") + ".dmp").toStdWString().c_str(),
//                GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

//    if( NULL == hFile || INVALID_HANDLE_VALUE == hFile )
//        return EXCEPTION_EXECUTE_HANDLER;

//    MINIDUMP_EXCEPTION_INFORMATION eInfo;
//    eInfo.ThreadId = GetCurrentThreadId();
//    eInfo.ExceptionPointers = ExceptionInfo;
//    eInfo.ClientPointers = FALSE;

//    MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile,
//                       MiniDumpNormal, &eInfo, NULL, NULL);
//    CloseHandle( hFile );
//    unsigned long code = ExceptionInfo->ExceptionRecord->ExceptionCode;
//    qCritical().noquote().noquote() <<
//                                       QObject::tr("^Fatal error! Here is some info about it: description: %1; type: %2; code: %3")
//                                       .arg(description).arg(type).arg(code);
//    errorAbort(1);
//    return EXCEPTION_EXECUTE_HANDLER;
//}
//#elif LINUX
//void posix_signal_handler(int sig, siginfo_t *siginfo, void *context)
//{
//    QString type, description;
//    switch(sig)
//    {
//    case SIGSEGV:
//        type = "SIGSEGV"; description = "Segmentation Fault";
//        break;
//    case SIGINT:
//        return;
//    case SIGFPE:
//        switch(siginfo->si_code)
//        {
//        case FPE_INTDIV:
//            type = "SIGFPE"; description = "integer divide by zero";
//            break;
//        case FPE_INTOVF:
//            type = "SIGFPE"; description = "integer overflow";
//            break;
//        case FPE_FLTDIV:
//            type = "SIGFPE"; description = "floating-point divide by zero";
//            break;
//        case FPE_FLTOVF:
//            type = "SIGFPE"; description = "floating-point overflow";
//            break;
//        case FPE_FLTUND:
//            type = "SIGFPE"; description = "floating-point underflow";
//            break;
//        case FPE_FLTRES:
//            type = "SIGFPE"; description = "floating-point inexact result";
//            break;
//        case FPE_FLTINV:
//            type = "SIGFPE"; description = "floating-point invalid operation";
//            break;
//        case FPE_FLTSUB:
//            type = "SIGFPE"; description = "subscript out of range";
//            break;
//        default:
//            type = "SIGFPE"; description = "Arithmetic Exception";
//            break;
//        }
//    case SIGILL:
//        switch(siginfo->si_code)
//        {
//        case ILL_ILLOPC:
//            type = "SIGILL"; description = "illegal opcode";
//            break;
//        case ILL_ILLOPN:
//            type = "SIGILL"; description = "illegal operand";
//            break;
//        case ILL_ILLADR:
//            type = "SIGILL"; description = "illegal addressing mode";
//            break;
//        case ILL_ILLTRP:
//            type = "SIGILL"; description = "illegal trap";
//            break;
//        case ILL_PRVOPC:
//            type = "SIGILL"; description = "privileged opcode";
//            break;
//        case ILL_PRVREG:
//            type = "SIGILL"; description = "privileged register";
//            break;
//        case ILL_COPROC:
//            type = "SIGILL"; description = "coprocessor error";
//            break;
//        case ILL_BADSTK:
//            type = "SIGILL"; description = "internal stack error";
//            break;
//        default:
//            type = "SIGILL"; description = "Illegal Instruction";
//            break;
//        }
//        break;
//    case SIGTERM:
//        type = "SIGTERM"; description = "a termination request was sent to the program";
//        break;
//    case SIGABRT:
//        type = "SIGABRT"; description = "usually caused by an abort() or assert()";
//        break;
//    default:
//        break;
//    }
//    qCritical().noquote().noquote() <<
//                                       QObject::tr("^Fatal error! Here is some info about it: description: %1; "
//                                                   "exception code: %2; exception info code %3")
//                                       .arg(description).arg(sig).arg(siginfo->si_code);
//    errorAbort(1);
//}
//#endif

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
//        return;
//    case SIGSEGV:
//        type = "SIGSEGV"; description = "segmentation fault";
//        break;
//    case SIGTERM:
//    default:
//        type = "SIGTERM"; description = "a termination request was sent to the program";
//        break;
//    }
//    qCritical().noquote().noquote() << QObject::tr("^Fatal error: description: %1; type: %2; code: %3")
//                                       .arg(description, type, QString::number(sig));
//    signal(sig, SIG_DFL);
//    errorAbort(1);
//}


//void set_signal_handler()
//{
//#if WIN
//    SetUnhandledExceptionFilter(windows_exception_handler);
//#elif LINUX
//    {
//        struct sigaction sig_action = {};
//        sig_action.sa_sigaction = posix_signal_handler;
//        sigemptyset(&sig_action.sa_mask);

//        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

//        if (sigaction(SIGSEGV, &sig_action, NULL) != 0) { err(1, "sigaction"); }
//        if (sigaction(SIGFPE,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
//        if (sigaction(SIGINT,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
//        if (sigaction(SIGILL,  &sig_action, NULL) != 0) { err(1, "sigaction"); }
//        if (sigaction(SIGTERM, &sig_action, NULL) != 0) { err(1, "sigaction"); }
//        if (sigaction(SIGABRT, &sig_action, NULL) != 0) { err(1, "sigaction"); }
//    }
//#endif
//    signal(SIGABRT, all_handler);
//    signal(SIGFPE,  all_handler);
//    signal(SIGILL,  all_handler);
//    signal(SIGINT,  all_handler);
//    signal(SIGSEGV, all_handler);
//    signal(SIGTERM, all_handler);
////    std::set_terminate([=]() {
////        qCritical().noquote() << QObject::tr("Unknown fatal error!");
////        exit(3);
////    });
//}

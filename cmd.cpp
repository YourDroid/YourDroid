#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>
#include <QtGlobal>
#include <QProcess>
#if WIN
#include <windows.h>
//#include <Processthreadsapi.h>
#include <winbase.h>
//#include <TCHAR.H>
#include <w32api.h>
#endif

QPair<int, QString> cmd::exec(QString command) {
//#if LINUX
//    int _res = 22;
//    FILE *trm = popen((command + QString(" 2>&1")).toStdString().c_str(), "r");
//    char buffer[128];
//    while(!feof(trm)) {
//         if(fgets(buffer, 128, trm) != NULL)
//             _output += buffer;
//    }
//    _res = pclose(trm);
//#elif WIN
//    DWORD _res = 22;
//    QString strResult;
//       HANDLE hPipeRead, hPipeWrite;

//       SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
//       saAttr.bInheritHandle = TRUE;   //Pipe handles are inherited by child process.
//       saAttr.lpSecurityDescriptor = NULL;

//       // Create a pipe to get results from child's stdout.
//       if ( !CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0) ) {
//           qCritical() << QObject::tr("Cannot create thread");
//           ret();
//       }
//       qDebug() << QObject::tr("Thread successfully created");

//       STARTUPINFO si = { sizeof(STARTUPINFO) };
//       si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//       si.hStdOutput  = hPipeWrite;
//       si.hStdError   = hPipeWrite;
//       si.wShowWindow = SW_HIDE;       // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.

//       PROCESS_INFORMATION pi  = { 0 };

//       BOOL fSuccess = CreateProcessW( NULL, (LPWSTR)command.toStdWString().c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
//       if (! fSuccess)
//       {
//           CloseHandle( hPipeWrite );
//           CloseHandle( hPipeRead );
//           qCritical() << QObject::tr("Cannot create procces");
//           ret();
//       }
//       qDebug() << QObject::tr("Proccess successfully created");

//       int i = 0;
//       bool bProcessEnded = false;
//       for (; !bProcessEnded ;)
//       {
//           // Give some timeslice (50ms), so we won't waste 100% cpu.
//           bProcessEnded = WaitForSingleObject( pi.hProcess, 50) == WAIT_OBJECT_0;

//           // Even if process exited - we continue reading, if there is some data available over pipe.
//           for (;;)
//           {
//               i++;
//               char buf[1024];
//               DWORD dwRead = 0;
//               DWORD dwAvail = 0;

//               if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
//                   break;

//               if (!dwAvail) // no data available, return
//                   break;

//               DWORD temp = sizeof(buf) - 1;
//               if (!::ReadFile(hPipeRead, buf, qMin(temp, dwAvail), &dwRead, NULL) || !dwRead)
//                   // error, the child process might ended
//                   break;

//               buf[dwRead] = 0;
//               strResult += buf;
//           }
//       } //for
//       qDebug() << QObject::tr("Reading finished by %1 steps").arg(i);

//    if(GetExitCodeProcess(pi.hProcess, &_res) == 0) _res = GetLastError();
//       CloseHandle( hPipeWrite );
//       CloseHandle( hPipeRead );
//       CloseHandle( pi.hProcess );
//       CloseHandle( pi.hThread );
//    _output = strResult;
//#endif
    qDebug() << QObject::tr("Executing %1").arg(command);
    QString _output = "";
    QProcess proc;
    bool succes = true;
    proc.start(command);
    int _res = 22;
    if(!proc.waitForStarted()) {
        qCritical() << QObject::tr("Could not start");
        succes = false;
    }
    else if(!proc.waitForFinished()) {
        qCritical() << QObject::tr("Could not finish");
        succes = false;
    }
    else if(proc.exitStatus() == QProcess::CrashExit) {
        succes = false;
        qCritical() << QObject::tr("Application has crashed");
    }
    if(succes) {
        _output = proc.readAllStandardOutput().data();
        _res = proc.exitCode();
    }

    if(succes) qDebug() << QObject::tr("Executing ended succesfull");
    else qCritical() << QObject::tr("Executing ended unsuccesfull");
    if(_res) {
        qWarning() << _output;
        qWarning() << QObject::tr("Returned value is ") << _res;
    }
    else {
        qDebug() << _output;
        qDebug() << QObject::tr("Returned value is ") << _res;
    }
    return QPair<int, QString>(_res, _output);
}

















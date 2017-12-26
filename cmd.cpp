#include "cmd.h"
#include "log.h"
#include <QFile>
#include <stdlib.h>
#include <QtGlobal>
#if WIN
#include <Processthreadsapi.h>
#include <winbase.h>
#include <TCHAR.H>
#include <w32api.h>
#endif

QPair<int, QString> cmd::exec(QString command) {
    qDebug() << qApp->translate("log", "Executing ") + command;
    QString _output = "";
#define ret() if(_res) { \
    qCritical() << _output; \
    qCritical() << QObject::tr("Error while executing!") << QObject::tr(" Returned value is ") << _res; \
} \
else { \
    qDebug() << _output; \
    qDebug() << QObject::tr("Command executed succesful") << QObject::tr(" Returned value is ") << _res; \
} \
return QPair<int, QString>(_res, _output);
//#if OS == 1
//    QString home = getenv("USERPROFILE");
//    QString strcmd = command + qApp->translate("log", ">") + home + qApp->translate("log", "\\temp_cmd 2>&1");
//    QFile tempCmd(home + qApp->translate("log", "\\temp_cmd"));
//#elif OS == 0
//    QString home = getenv("home");
//    QString strcmd = command + qApp->translate("log", " &> ") + home + qApp->translate("log", "/temp_cmd");
//    QFile tempCmd(home + qApp->translate("log", "/temp_cmd"));
//#endif
//    _res = system(strcmd.toStdString().c_str());
//    if (tempCmd.open(QIODevice::ReadOnly))
//    {
//        _output = tempCmd.readAll();
//        tempCmd.close();
//    }
#if LINUX
    int _res = 22;
    FILE *trm = popen((command + QString(" 2>&1")).toStdString().c_str(), "r");
    char buffer[128];
    while(!feof(trm)) {
         if(fgets(buffer, 128, trm) != NULL)
             _output += buffer;
    }
    _res = pclose(trm);
#elif WIN
    DWORD _res = 22;
    QString strResult;
       HANDLE hPipeRead, hPipeWrite;

       SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
       saAttr.bInheritHandle = TRUE;   //Pipe handles are inherited by child process.
       saAttr.lpSecurityDescriptor = NULL;

       // Create a pipe to get results from child's stdout.
       if ( !CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0) ) {
           qCritical() << QObject::tr("Cannot create thread");
           ret();
       }
       qDebug() << QObject::tr("Thread successfully created");

       STARTUPINFO si = { sizeof(STARTUPINFO) };
       si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
       si.hStdOutput  = hPipeWrite;
       si.hStdError   = hPipeWrite;
       si.wShowWindow = SW_HIDE;       // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.

       PROCESS_INFORMATION pi  = { 0 };

       BOOL fSuccess = CreateProcessW( NULL, (LPWSTR)command.toStdWString().c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
       if (! fSuccess)
       {
           CloseHandle( hPipeWrite );
           CloseHandle( hPipeRead );
           qCritical() << QObject::tr("Cannot create procces");
           ret();
       }
       qDebug() << QObject::tr("Proccess successfully created");

       int i = 0;
       bool bProcessEnded = false;
       for (; !bProcessEnded ;)
       {
           // Give some timeslice (50ms), so we won't waste 100% cpu.
           bProcessEnded = WaitForSingleObject( pi.hProcess, 50) == WAIT_OBJECT_0;

           // Even if process exited - we continue reading, if there is some data available over pipe.
           for (;;)
           {
               i++;
               char buf[1024];
               DWORD dwRead = 0;
               DWORD dwAvail = 0;

               if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
                   break;

               if (!dwAvail) // no data available, return
                   break;

               DWORD temp = sizeof(buf) - 1;
               if (!::ReadFile(hPipeRead, buf, qMin(temp, dwAvail), &dwRead, NULL) || !dwRead)
                   // error, the child process might ended
                   break;

               buf[dwRead] = 0;
               strResult += buf;
           }
       } //for
       qDebug() << QObject::tr("Reading finished by %1 steps").arg(i);

    if(GetExitCodeProcess(pi.hProcess, &_res) == 0) _res = GetLastError();
       CloseHandle( hPipeWrite );
       CloseHandle( hPipeRead );
       CloseHandle( pi.hProcess );
       CloseHandle( pi.hThread );
    _output = strResult;

    //DWORD result;
//    _res = *result;

//    int lsOutPipe[2];
//    pipe(lsOutPipe);

//    //Fork to two processes.
//    pid_t lsPid=fork();

//    //Check if I'm the child or parent.
//    if ( 0 == lsPid )
//    {//I'm the child.
//      //Close the read end of the pipe.
//      close(lsOutPipe[0]);

//      //Make the pipe be my stdout.
//      dup2(lsOutPipe[1],STDOUT_FILENO);

//      //Replace my self with ls (using one of the exec() functions):
//      exec((command + QString(" 2>&1")).toStdString().c_str());//This never returns.
//    } // if

//    //I'm the parent.
//    //Close the read side of the pipe.
//    _res = close(lsOutPipe[1]);

//    //Read stuff from ls:
//    char buffer[1024];
//    int bytesRead;
//    do
//    {
//      bytesRead = read(emacsInPipe[0], buffer, 1024);

//      // Do something with the read information.
//      if (bytesRead > 0) _output += buffer;
//    } while (bytesRead > 0);
#endif
    qDebug() << QObject::tr("Executing ended");
    ret();
}

//QPair<int, QString> cmd::exec() {
//    return exec(_command);
//}

















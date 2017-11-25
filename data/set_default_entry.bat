chcp 1251
bcdedit /enum firmware>%~dp0enum.txt
set lgfl=%~dp0enum.txt
::указать выходной файл (куда копировать предварительно отфильрованные две строки, можно оптимизировать, не создавая всех логов и результатов поиска, перенаправив сразу все выводы в след. команды, это после отладки)
set nwlg=%~dp0outlog.txt
::указать кол-во вывода строк до искомой
set num1=3
::указать кол-во вывода строк после искомой
set num2=0

set word=EFI
set word=Hard
set word=Drive
cls
@echo off
setlocal EnableDelayedExpansion

for /f "delims=: tokens=1" %%a in ('findstr /n /r "%word%" %lgfl%') do (call :pars %%a)
pause

SetLocal EnableExtensions
set file=%nwlg%
For /F "UseBackQ tokens=1-2 delims==( " %%a in ("%file%") do if "%%a"=="identifier" set guid=%%b
For /F "UseBackQ tokens=1-2 delims==( " %%a in ("%file%") do if "%%a"=="идентификатор" set guid=%%b
echo guid=%guid%
pause
::после отладки раскомментировать эту команду
::bcdedit /set {fwbootmgr} displayorder %guid% /addfirst
pause

exit /b

:pars
set /a isum=%num1%+%num2%+1
if %num1% lss %1 (
set /a num=%1-%num1%-1
set "skp=skip=!num! "
) else (
set "skp="
set /a isum=%num2%+%1
)
for /f "%skp%tokens=*" %%a in (%lgfl%) do (
set /a isum-=1
echo %%a
echo %%a>>%nwlg%
if !isum! lss 1 exit /b)
exit /b
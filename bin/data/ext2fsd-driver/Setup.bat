@echo off

if %1.==. goto auto_install
if %1.==help. goto help
if %1.==install. goto auto_install
if %2.==. goto help

set ext2os=%1
set ext2arch=%2
goto manual_install

:auto_install
set ext2os=wxp
if %ext2os%. == . goto unknown_os

if "%PROCESSOR_ARCHITECTURE%"=="AMD64" set ext2arch=amd64
if "%PROCESSOR_ARCHITEW6432%"=="AMD64" set ext2arch=amd64
if "%PROCESSOR_ARCHITECTURE%"=="X86" set ext2arch=i386
if "%PROCESSOR_ARCHITECTURE%"=="x86" set ext2arch=i386
if %ext2arch%. == . goto unknown_os

:manual_install

if not exist %ext2os%\%ext2arch%\ext2fsd.sys goto file_not_exist
copy %ext2os%\%ext2arch%\ext2fsd.sys . >nul

:install
copy .\ext2fsd.inf %windir%\inf > NUL
rundll32.exe setupapi.dll,InstallHinfSection DefaultInstall 132 .\ext2fsd.inf
net start ext2fsd

goto exit

:file_not_exist
echo Error: file %ext2os%\%ext2arch%\ext2fsd.sys does NOT exist !
goto exit

:unknown_os
echo can not determine OS version. please setup manually.
:help
echo -------------------------------------------------------------------------------
echo Help on ext2fsd setup utility:
echo -------------------------------------------------------------------------------
echo  Automatical installation:
echo   setup install          - automatically check os version and install ext2fsd
echo -------------------------------------------------------------------------------
echo  Manual installation:
echo   setup w2k  i386        - Install Ext2Fsd to Windows 2000 X86
echo   setup wxp  i386        - Install Ext2Fsd to Windows XP/2003/WIN7/WIN8 X86
echo   setup wxp  amd64       - Install Ext2Fsd to Windows XP/2003/WIN7/WIN8 AMD64
echo -------------------------------------------------------------------------------
:exit
if exist ext2fsd.sys del ext2fsd.sys
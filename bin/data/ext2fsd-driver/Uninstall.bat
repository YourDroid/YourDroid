@echo off
rundll32.exe setupapi.dll,InstallHinfSection DefaultUninstall 132 .\ext2fsd.inf
del %windir%\inf\ext2fsd.*

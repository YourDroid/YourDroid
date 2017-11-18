mkdir binary
copy %~dp0release\yourdroid.exe %~dp0binary\
mkdir binary\data
xcopy /E %~dp0data %~dp0binary\data
windeployqt %~dp0binary\yourdroid.exe
pause
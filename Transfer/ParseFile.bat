set data1=
set data2=

if not exist %2 goto :Parse

call :Time1 %1
call :Time2 %2

if %data1:~0,4% gtr %data2:~0,4% goto :Parse
if %data1:~0,4% lss %data2:~0,4% goto :eof

if %data1:~5,2% gtr %data2:~5,2% goto :Parse
if %data1:~5,2% lss %data2:~5,2% goto :eof

if %data1:~8,2% gtr %data2:~8,2% goto :Parse
if %data1:~8,2% lss %data2:~8,2% goto :eof

if %data1:~11,2% gtr %data2:~11,2% goto :Parse
if %data1:~11,2% lss %data2:~11,2% goto :eof

if %data1:~14,2% geq %data2:~14,2% goto :Parse
if %data1:~14,2% lss %data2:~14,2% goto :eof

goto :Parse

goto :eof

:Time1
set data1=%~t1 %~n1%~x1
goto :eof

:Time2
set data2=%~t1 %~n1%~x1
goto :eof

:Parse
echo 生成协议%1
.\SDParser.exe %1
if errorlevel 1 echo 协议编译失败,请修改协议文件重新编译!
goto :eof
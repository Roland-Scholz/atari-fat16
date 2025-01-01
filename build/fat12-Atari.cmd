@echo off

set PLATFORM=4
call var-def.cmd


call :compile_module fat16 0D00
if NOT %result%==0 goto ende

rem call :compile_module dup 1b00
rem if NOT %result%==0 goto ende

call :compile_module bootsec A800
if NOT %result%==0 goto ende

cd %REL%
copy ..\%RES%\boot.atr .
copy /Y /B fat16.com + dup.com dos.sys

..\%TOOLS%\xfddos -i boot.atr dos.sys
..\%TOOLS%\xfddos -i boot.atr bootsec.com

echo on

mkdir obj
mkdir lst

move *.lst lst > nul
move *.o obj > nul
move *.a obj > nul

copy boot.atr e:


:ende

pause
goto eof


:compile_module
%CC65%\ca65 -DPLATFORM=%PLATFORM% -l  %REL%\%1.lst %SRC%\%1.a65 -I %INC% -I %COMMON%\inc -o %REL%\%1.o
set result=%ERRORLEVEL%

if %result%==0 (

	%CC65%\ld65 -t none %REL%\%1.o -o %REL%\%1.a
	%CC65%\bin2hex %REL%\%1.a %REL%\%1.hex -o %2
	java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar Obj2Com %REL%\%1.a %2
)

:eof
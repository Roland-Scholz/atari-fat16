@echo off

set PLATFORM=3
call var-def.cmd

rmdir /S /Q %REL%
mkdir %REL%

rem call :compile_module fat16 7300
rem if NOT %result%==0 goto ende

call :compile_module dup 0A00
if NOT %result%==0 goto ende

cd %REL%

mkdir obj
mkdir lst

move *.lst lst > nul
move *.o obj > nul
move *.a obj > nul

:ende

pause
goto eof


:compile_module
%CC65%\ca65 -DPLATFORM=%PLATFORM% -l %REL%\%1.lst %SRC%\%1.a65 -I %INC% -I %COMMON%\inc -I %MY65816%\inc -o %REL%\%1.o
set result=%ERRORLEVEL%

if %result%==0 (

	%CC65%\ld65 -C homebrew.cfg %REL%\%1.o -o %REL%\%1.a
	%CC65%\bin2hex %REL%\%1.a %REL%\%1.hex -o %2
	java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar Obj2Com %REL%\%1.a %2
)

:eof
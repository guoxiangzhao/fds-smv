@echo off
set intelbin="%IFORT_COMPILER14%\bin"

IF "%SETUP_IFORT_COMPILER_64%"=="1" GOTO envexist

set SETUP_IFORT_COMPILER_64=1

echo Setting up compiler environment
call %intelbin%\ifortvars intel64

:envexist

erase *.obj *.mod *.exe
make VPATH=".." -f ..\makefile intel_win_64
pause

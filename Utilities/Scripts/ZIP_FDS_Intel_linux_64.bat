@echo off

Rem Batch file used to create a self-extracting archive containing FDS

set envfile=%userprofile%\fds_smv_env.bat
IF EXIST %envfile% GOTO endif_envexist
echo ***Fatal error.  The environment setup file %envfile% does not exist. 
echo Create a file named %envfile% and use SMV/scripts/fds_smv_env_template.bat
echo as an example.
echo.
echo Aborting now...
pause>NUL
goto:eof

:endif_envexist

call %envfile%

%svn_drive%
cd %svn_root%\Utilities\Makefile

set fdsroot=fds_%fds_version%_%fds_revision%_linux64
set togoogle=%svn_root%\Utilities\uploads
set fdsrootdir=%togoogle%\%fdsroot%
set scriptdir=%linux_svn_root%/Utilities/Scripts
mkdir %fdsrootdir%

echo.
echo moving 64 bit linux fds files to an interim directory
plink %svn_logon% %scriptdir%/bundle_linux_64.csh %scriptdir%

echo.
echo downloading 64 bit linux fds files
pscp %svn_logon%:%scriptdir%/uploadds/fds_intel_linux_64 %fdsrootdir%\fds_intel_linux_64
pscp %svn_logon%:%scriptdir%/uploads/fds_mpi_intel_linux_64 %fdsrootdir%\fds_mpi_intel_linux_64

echo.
echo winzipping distribution directory
cd %fdsrootdir%
if exist ..\%fdsroot%.zip erase ..\%fdsroot%.zip
wzzip -a -r -P ..\%fdsroot%.zip *
cd ..

echo %fdsroot%.zip located in %togoogle%
pause

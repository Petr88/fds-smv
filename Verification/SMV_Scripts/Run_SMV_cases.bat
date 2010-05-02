@echo off
set svn_drive=d:

set SCRIPT_DIR=%CD%
set BASEDIR=%CD%\..
set SVNROOT=%BASEDIR%\..\

set FDS=%SVNROOT%\FDS_Compilation\intel_win_32\fds5_win_32
Rem set FDS=background -d 1 -u 90 %SVNROOT%\FDS_Compilation\intel_win_64\fds5_win_64

set RUNFDS=call %SVNROOT%\Utilities\Scripts\runfds_win32.bat

echo You are about to run the Smokeview Verification Test Suite.
pause > Nul

cd %BASEDIR%

echo "smokeview test cases begin" > smokeview.begin

Rem create a text file containing the FDS5 version used to run these tests.
Rem This file is included in the smokeview user's guide

set smvug="%CD%\..\Manuals\SMV_5_User_Guide\"
echo | %FDS% 2> "%smvug%\figures\fds5.version"

call %SCRIPT_DIR%\SMV_Cases.bat

cd %BASEDIR%
echo "smokeview test cases end" > smokeview.end

pause

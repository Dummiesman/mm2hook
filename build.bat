@ECHO off

REM check if script is running inside a shell
SET IN_SHELL=1

SET "explorercmdx=%comspec% /c xx%~0x x"
SET "shellcmdx=%cmdcmdline:"=x%"

IF /I "%explorercmdx%"=="%shellcmdx%" (
    SET IN_SHELL=0
)

REM construct default paths to MSVC build tools
SET "BUILDTOOLS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\"
SET "VCVARS=VC\Auxiliary\Build\vcvarsall.bat"
SET "MSBUILD=MSBuild\Current\Bin\amd64\MSBuild.exe"

SET TARGET_BUILD=release

REM iterate over input arguments
:input
IF "%~1"=="" GOTO endinput

IF "%~1"=="-d" SET TARGET_BUILD=debug
IF "%~1"=="--debug" SET TARGET_BUILD=debug

IF "%~1"=="-?" GOTO usage
IF "%~1"=="--help" GOTO usage

SHIFT
GOTO input

REM print usage instructions
:usage
ECHO -? or --help
ECHO -d or --debug

EXIT /b 0

REM input processing finished
:endinput
SET "VCVARS_PATH=%BUILDTOOLS_PATH%%VCVARS%"
SET "MSBUILD_PATH=%BUILDTOOLS_PATH%%MSBUILD%"

call "%VCVARS_PATH%" x86 10.0.18362.0
"%MSBUILD_PATH%" MM2Hook.sln /p:configuration=%TARGET_BUILD% /p:platform=x86
SET "EXIT_CODE=%ERRORLEVEL%"

ECHO.
ECHO Finished %TARGET_BUILD% build, exit code %EXIT_CODE%
ECHO.

IF %IN_SHELL%==0 (
    ECHO Press any key to exit...
    PAUSE>nul
)

EXIT /b %EXIT_CODE%

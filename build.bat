@ECHO off

REM check if script is running inside a shell
SET IN_SHELL=1

SET "explorercmdx=%comspec% /c xx%~0x x"
SET "shellcmdx=%cmdcmdline:"=x%"

IF /I "%explorercmdx%"=="%shellcmdx%" (
    SET IN_SHELL=0
)

REM default paths for MSVC build tools
SET "MSVC_BUILDTOOLS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\"
SET "VCVARS=VC\Auxiliary\Build\vcvarsall.bat"
SET "MSBUILD=MSBuild\Current\Bin\amd64\MSBuild.exe"

REM look for MSVC install
SET "MSVC_2022_X86_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\"
SET "MSVC_2022_X64_PATH=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\"
SET "MSVC_2022_ENTERPRISE_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\"

if exist "%MSVC_2022_ENTERPRISE_PATH%" (
    SET "MSVC_BUILDTOOLS_PATH=%MSVC_2022_ENTERPRISE_PATH%"
)

if exist "%MSVC_2022_X64_PATH%" (
    SET "MSVC_BUILDTOOLS_PATH=%MSVC_2022_X64_PATH%"
)

if exist "%MSVC_2022_X86_PATH%" (
    SET "MSVC_BUILDTOOLS_PATH=%MSVC_2022_X86_PATH%"
)

SET TARGET_BUILD=Release
SET CREATE_ZIP=0

REM iterate over input arguments
:input
IF "%~1"=="" GOTO endinput

IF "%~1"=="-d" SET TARGET_BUILD=Debug
IF "%~1"=="--debug" SET TARGET_BUILD=Debug

IF "%~1"=="-z" SET CREATE_ZIP=1
IF "%~1"=="--zip" SET CREATE_ZIP=1

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

CALL "%VCVARS_PATH%" x86 10.0.18362.0
"%MSBUILD_PATH%" MM2Hook.sln /p:configuration=%TARGET_BUILD% /p:platform=x86
SET "EXIT_CODE=%ERRORLEVEL%"

ECHO.
ECHO Finished %TARGET_BUILD% build, exit code %EXIT_CODE%

IF %EXIT_CODE%==0 IF %CREATE_ZIP%==1 (
    ECHO.
    ECHO Creating release zip...
    TAR -a -c -C "bin\%TARGET_BUILD%" -v -f "bin\%TARGET_BUILD%\mm2hook.zip" MM2Hook.dll MM2HookLoader.dll mm2hook.ini
    ECHO Release zip created!
)

IF %IN_SHELL%==0 (
    ECHO.
    ECHO Press any key to exit...
    PAUSE>nul
)

EXIT /b %EXIT_CODE%

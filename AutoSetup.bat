@echo off
color 0B
title Dev-C++ Graphics Setup

:: 1. Request Administrator Rights (Keep window open forever with /k)
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [INFO] Requesting Administrator Privileges...
    powershell -NoProfile -Command "Start-Process cmd.exe -ArgumentList '/k \"\"%~f0\"\"' -Verb RunAs"
    exit /b
)

cd /d "%~dp0"
cls

echo ======================================================================
echo           Dev-C++ 5.11 BGI Graphics Automated Installer
echo ======================================================================
echo.

:: 2. Check if Dev-C++ is Running
tasklist /FI "IMAGENAME eq devcpp.exe" 2>nul | find /I /N "devcpp.exe" >nul
if %errorlevel% equ 0 (
    color 0E
    echo [WARNING] Dev-C++ is currently running.
    echo Files cannot be updated while Dev-C++ is open.
    echo.
    echo Press any key to close Dev-C++ and continue...
    pause >nul
    taskkill /F /IM devcpp.exe >nul 2>&1
    echo [OK] Dev-C++ closed successfully.
    echo.
    color 0B
)

:: 3. Verify Source Files
echo [*] Checking local source files...
if not exist "%~dp0graphics.h" goto :err_graphics
if not exist "%~dp0winbgim.h"  goto :err_winbgim
if not exist "%~dp0libbgi.a"   goto :err_libbgi
echo [OK] All 3 source files found.
echo.

:: 4. Locate Dev-Cpp Root
echo [*] Searching for Dev-Cpp installation...
set "DEV_ROOT="

if exist "C:\Program Files (x86)\Dev-Cpp\MinGW64" set "DEV_ROOT=C:\Program Files (x86)\Dev-Cpp"
if not defined DEV_ROOT if exist "C:\Program Files\Dev-Cpp\MinGW64" set "DEV_ROOT=C:\Program Files\Dev-Cpp"
if not defined DEV_ROOT if exist "D:\Program Files (x86)\Dev-Cpp\MinGW64" set "DEV_ROOT=D:\Program Files (x86)\Dev-Cpp"
if not defined DEV_ROOT if exist "D:\Dev-Cpp\MinGW64" set "DEV_ROOT=D:\Dev-Cpp"
if not defined DEV_ROOT if exist "C:\Dev-Cpp\MinGW64" set "DEV_ROOT=C:\Dev-Cpp"

if not defined DEV_ROOT (
    echo [!] Dev-Cpp not found in default paths.
    set /p "DEV_ROOT=Enter Dev-Cpp folder (Press Enter for C:\Program Files (x86)\Dev-Cpp): "
)
if "%DEV_ROOT%"=="" set "DEV_ROOT=C:\Program Files (x86)\Dev-Cpp"

if not exist "%DEV_ROOT%\MinGW64" goto :err_mingw

echo [OK] Dev-Cpp found at: %DEV_ROOT%
echo.

:: 5. Copy Files and Patch
echo [*] Deploying files...
echo.
echo %DEV_ROOT%
echo   \-- MinGW64

set "INC1=%DEV_ROOT%\MinGW64\x86_64-w64-mingw32\include"
set "INC2=%DEV_ROOT%\MinGW64\include"
set "LIB1=%DEV_ROOT%\MinGW64\x86_64-w64-mingw32\lib"
set "LIB2=%DEV_ROOT%\MinGW64\x86_64-w64-mingw32\lib32"
set "LIB3=%DEV_ROOT%\MinGW64\lib"
set "LIB4=%DEV_ROOT%\MinGW64\lib32"

:: Copy to INC1
if exist "%INC1%" (
    copy /Y "%~dp0graphics.h" "%INC1%\" >nul
    copy /Y "%~dp0winbgim.h" "%INC1%\" >nul
    echo        \-- x86_64-w64-mingw32\include
    echo             +-- [COPIED] graphics.h
    echo             +-- [COPIED] winbgim.h
    powershell -NoProfile -Command "(Get-Content '%INC1%\graphics.h') -replace 'int right=0, int bottom=0', 'int right=INT_MAX, int bottom=INT_MAX' | Set-Content '%INC1%\graphics.h'"
    echo             +-- [PATCHED] graphics.h Line 302 conflict resolved
)

:: Copy to INC2 (Root include)
if exist "%INC2%" (
    copy /Y "%~dp0graphics.h" "%INC2%\" >nul
    copy /Y "%~dp0winbgim.h" "%INC2%\" >nul
    powershell -NoProfile -Command "(Get-Content '%INC2%\graphics.h') -replace 'int right=0, int bottom=0', 'int right=INT_MAX, int bottom=INT_MAX' | Set-Content '%INC2%\graphics.h'"
)

:: Copy to LIB directories
if exist "%LIB1%" (
    copy /Y "%~dp0libbgi.a" "%LIB1%\" >nul
    echo        \-- x86_64-w64-mingw32\lib
    echo             +-- [COPIED] libbgi.a
)
if exist "%LIB2%" (
    copy /Y "%~dp0libbgi.a" "%LIB2%\" >nul
    echo        \-- x86_64-w64-mingw32\lib32
    echo             +-- [COPIED] libbgi.a
)
if exist "%LIB3%" copy /Y "%~dp0libbgi.a" "%LIB3%\" >nul
if exist "%LIB4%" copy /Y "%~dp0libbgi.a" "%LIB4%\" >nul

:: 6. Finish
echo.
color 0A
echo ======================================================================
echo                  SETUP COMPLETED SUCCESSFULLY!
echo ======================================================================
echo.
echo Inside Dev-C++ IDE Checklist:
echo  1. Change Compiler to:  [ TDM-GCC 4.9.2 32-bit Release ]
echo  2. Tools -^> Compiler Options -^> Add these Linker flags:
echo.
echo     -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
echo.
echo ======================================================================
echo Done! You can now close this window.
pause >nul
exit /b

:: Error Handlers
:err_graphics
color 0C
echo [ERROR] graphics.h was not found in this folder.
pause
exit /b

:err_winbgim
color 0C
echo [ERROR] winbgim.h was not found in this folder.
pause
exit /b

:err_libbgi
color 0C
echo [ERROR] libbgi.a was not found in this folder.
pause
exit /b

:err_mingw
color 0C
echo [ERROR] MinGW64 was not found inside: %DEV_ROOT%
pause
exit /b

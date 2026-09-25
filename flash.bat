@echo off
setlocal enabledelayedexpansion

if not "%~1"=="" (
    set "port=%~1"
) else (
    for /f "tokens=2 delims=:, " %%a in ('type "..\board_config.json" ^| findstr /c:"\"port\""') do (
        set "port=%%a"
        set "port=!port: =!"
        set "port=!port:"=!"
    )
)

if not defined port (
    echo Error: Unable to read serial port. Please verify the connection.
    exit /b 1
)

if not "%~2"=="" (
    set "fw_file=%~2"
) else (
    set "fw_file=out\main.ihx"
)

if not exist "%fw_file%" (
    echo Error: Firmware file "%fw_file%" does not exist.
    exit /b 1
)

if "%~1"=="" (
    if not exist "..\board_config.json" (
        echo Error: ..\board_config.json not found. Please pass the COM port explicitly.
        echo Usage: flash.bat COM3 [firmware]
        exit /b 1
    )
)

rem ===== 选择可用的 python（优先仓库内嵌版，其次系统 python） =====
set "PY=%~dp0..\toolchains\python-3.8.10-embed-win32\python.exe"
if not exist "%PY%" set "PY=python"

"%PY%" tools\flash.py %port% "%fw_file%"
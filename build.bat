@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

rem ===== 选择可用的 python（优先仓库内嵌版，其次系统 python） =====
set "PY=%~dp0..\toolchains\python-3.8.10-embed-win32\python.exe"
if not exist "%PY%" set "PY=python"

rem ===== 1. 中文字库转换：_main.c -> main.c =====
"%PY%" tools\stc-conv_hz.py
if errorlevel 1 (
    echo [X] 字库转换失败
    exit /b 1
)

rem ===== 2. 编译（不再依赖 make，本机未安装 make 也能构建） =====
if not exist out mkdir out
echo Compiling main.c...
sdcc.exe -I lib -I myLib --model-small --stack-auto --stack-loc 0x80 --out-fmt-ihx -o out\main.ihx main.c
if errorlevel 1 (
    echo [X] 编译失败
    exit /b 1
)

rem ===== 3. ihx -> hex / bin =====
echo Generating main.hex...
packihx.exe out\main.ihx > out\main.hex
echo Generating main.bin...
makebin.exe -s 65536 -p out\main.ihx out\main.bin

echo Compilation complete!
echo Output: out\main.*

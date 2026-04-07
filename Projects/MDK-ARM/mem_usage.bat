@echo off
:: mem_usage.bat -- Keil 构建后内存用量可视化工具
:: 本脚本与 .uvprojx 放在同一目录（MDK-ARM\）
::
:: 用法（Keil After Build 命令）: "%~dp0mem_usage.bat"
:: 无需传参，工程名自动从当前目录的 .uvprojx 文件名推断

:: 自动推断工程名：扫描本脚本所在目录下的 .uvprojx 文件
set PROJ_NAME=
for %%f in ("%~dp0*.uvprojx") do set PROJ_NAME=%%~nf

if "%PROJ_NAME%"=="" (
    echo [ERROR] 当前目录未找到 .uvprojx 文件，请将此脚本放入 MDK-ARM 目录
    exit /b 1
)

:: .map 文件在 MDK-ARM\<工程名>\子目录下
:: %~dp0 即本脚本所在目录（MDK-ARM\）
set MAP_FILE=%~dp0Listings\%PROJ_NAME%.map

powershell -ExecutionPolicy Bypass -File "%~dp0mem_usage.ps1" "%MAP_FILE%"

:: 将 PowerShell 退出码透传给 Keil
exit /b %ERRORLEVEL%

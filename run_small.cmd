@echo off
cd /d "%~dp0"
if not exist lga.exe call build_windows.cmd
if not exist lga.exe exit /b 1
lga.exe -w 60 -h 40 -n 500 -d 10 --rho-in 1.6 -v 10

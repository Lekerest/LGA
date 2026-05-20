@echo off
cd /d "%~dp0"
if not exist lga.exe call build_windows.cmd
if not exist lga.exe exit /b 1
lga.exe -w 256 -h 160 -n 1000 -d 100 --rho-in 1.8 -v 100

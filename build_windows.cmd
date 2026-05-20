@echo off
setlocal
cd /d "%~dp0"

set "CC=C:\msys64\mingw64\bin\gcc.exe"
if not exist "%CC%" set "CC=gcc"

echo Compiler: %CC%
"%CC%" -O2 -Wall -Wextra -std=c11 main.c lattice.c collision.c boundary.c grid_io.c render.c -o lga.exe -lm
if errorlevel 1 (
    echo.
    echo BUILD ERROR
    exit /b 1
)

echo.
echo BUILD OK
if exist lga.exe dir lga.exe
endlocal

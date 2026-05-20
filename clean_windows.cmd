@echo off
cd /d "%~dp0"
if exist lga.exe del /Q lga.exe
if exist tests.exe del /Q tests.exe
if exist grid.bin del /Q grid.bin
if exist test_grid.bin del /Q test_grid.bin
if exist frames rmdir /S /Q frames
if exist test_frames rmdir /S /Q test_frames
if exist *.o del /Q *.o
if exist gcc_out.txt del /Q gcc_out.txt
if exist gcc_err.txt del /Q gcc_err.txt
echo Clean OK

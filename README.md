# LGA Project

Запуск на Windows через GCC/MSYS2 без make.

## Сборка

```powershell
.\build_windows.cmd
```

Или вручную:

```powershell
C:\msys64\mingw64\bin\gcc.exe -O2 -Wall -Wextra -std=c11 main.c lattice.c collision.c boundary.c grid_io.c render.c -o lga.exe -lm
```

## Маленькая решетка с ANSI

```powershell
.\run_small.cmd
```

Или:

```powershell
.\lga.exe -w 60 -h 40 -n 500 -d 10 --rho-in 1.6 -v 10
```

## Большая решетка с PPM

```powershell
.\run_big.cmd
```

Кадры сохраняются в папку `frames`.

## Продолжить из grid.bin

```powershell
.\lga.exe -w 60 -h 40 -n 500 -d 10 --rho-in 1.6 -v 10 --resume
```

## Тесты

```powershell
.\test_windows.cmd
```

## Очистка

```powershell
.\clean_windows.cmd
```

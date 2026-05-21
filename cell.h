#ifndef CELL_H
#define CELL_H

#include <stdint.h>
#include <stdbool.h>

#define DIR_COUNT 6
#define REST_BIT 6
#define STATE_MASK 0x7F

#define DIR_E  0
#define DIR_NE 1
#define DIR_NW 2
#define DIR_W  3
#define DIR_SW 4
#define DIR_SE 5

typedef struct {
    uint8_t state;
    bool is_obstacle;
} Cell;

typedef struct {
    int width;              // Ширина решётки в клетках
    int height;             // Высота решётки в клетках
    uint64_t iters;         // Количество итераций симуляции
    int viz_interval;       // Интервал визуализации: каждые viz_interval итераций сохраняется PPM и выводится ANSI-картинка для маленькой решётки
    int diameter;           // Диаметр цилиндра-препятствия в клетках
    unsigned int seed;      // Seed для генератора случайных чисел
    double rho_in;          // Плотность входного потока слева
    double rho_init;        // Начальная плотность случайного заполнения решётки
    bool resume;            // Флаг продолжения симуляции: false — начать новую симуляцию true  — загрузить состояние из grid.bin
    const char *bin_path;   // Путь к бинарному файлу состояния, обычно "grid.bin"
    const char *frames_dir; // Папка для сохранения PPM-кадров, обычно "frames"
} SimParams;

#endif

#ifndef CELL_H //защита от даблл включеня
#define CELL_H

#include <stdint.h>
#include <stdbool.h>

#define DIR_COUNT 6
#define REST_BIT 6
#define STATE_MASK 0x7F

#define DIR_E  0 //направления
#define DIR_NE 1
#define DIR_NW 2
#define DIR_W  3
#define DIR_SW 4
#define DIR_SE 5

typedef struct { //одна клетка решетки
    uint8_t state;
    bool is_obstacle;
} Cell;

typedef struct { //параметры симуляции
    int width;          // ширина решётки (флаг -w, по умолч. 256)
    int height;         // высота решётки (флаг -h, по умолч. 160)
    uint64_t iters;     // число итераций (флаг -n, по умолч. 1000)
    int viz_interval;   // интервал отрисовки в итерациях (флаг -v, по умолч. 100)
    int diameter;       // диаметр цилиндра в клетках (флаг -d, по умолч. 30)
    unsigned int seed;  // зерно для srand() (флаг -s, по умолч. time(NULL))
    double plot_in;     // плотность входного потока, p = plot_in/3 (флаг --plot-in, по умолч. 1.6)
    double plot_init;   // начальная плотность решётки, p = plot_init/7 (флаг --plot-init, по умолч. 1.0)
    bool resume;        // продолжить с сохранённого состояния (флаг --resume)
    const char *bin_path;    // путь к файлу сохранения (флаг --bin, по умолч. "grid.bin")
    const char *frames_dir;  // папка для PPM-кадров (фла
} SimParams;

#endif

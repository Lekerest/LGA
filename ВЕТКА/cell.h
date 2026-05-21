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
    int width;
    int height;
    uint64_t iters;
    int viz_interval;
    int diameter;
    unsigned int seed;
    double plot_in;
    double plot_init;
    bool resume;
    const char *bin_path;
    const char *frames_dir;
} SimParams;

#endif

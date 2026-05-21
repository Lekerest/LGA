#ifndef GRID_IO_H
#define GRID_IO_H

#include <stdint.h>
#include "cell.h"

#define GRID_MAGIC 0x3041474Cu

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint64_t iteration;
    double plot_in;
} GridHeader;

void grid_save(const Cell *grid, uint32_t w, uint32_t h,
               uint64_t iter, double plot_in, const char *path);

int grid_load(Cell *grid, uint32_t w, uint32_t h,
              uint64_t *iter, double *plot_in, const char *path);

#endif

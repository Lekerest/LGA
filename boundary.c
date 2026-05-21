#include <stdlib.h>
#include "boundary.h"
#include "lattice.h"
//поток слева(добавляем частицы которые летят вправую сторону)
void apply_inlet(Cell *grid, int w, int h, double plot_in) {
    double p = plot_in / 3.0;
    if (p < 0.0) p = 0.0;
    if (p > 1.0) p = 1.0;

    for (int y = 1; y < h - 1; y++) {
        int id = cell_index(0, y, w);
        if (grid[id].is_obstacle) continue;
        //рандомим направление
        if ((double)rand() / (double)RAND_MAX < p) grid[id].state |= (uint8_t)(1u << DIR_E);
        if ((double)rand() / (double)RAND_MAX < p) grid[id].state |= (uint8_t)(1u << DIR_NE);
        if ((double)rand() / (double)RAND_MAX < p) grid[id].state |= (uint8_t)(1u << DIR_SE);
    }
}
//сток справа(удаляем частицы отраженные от правой стенки)
void apply_outlet(Cell *grid, int w, int h) {
    for (int y = 0; y < h; y++) {
        int id = cell_index(w - 1, y, w);
        grid[id].state &= (uint8_t)~((1u << DIR_NW) | (1u << DIR_W) | (1u << DIR_SW));
    }
}

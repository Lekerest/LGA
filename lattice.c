#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lattice.h"
//смещение для правильеого представления четных и нечетных клеток решетки
static const int DX_EVEN[6] = { 1, 0, -1, -1, 0, 1 };
static const int DY_EVEN[6] = { 0, -1, -1, 0, 1, 1 };
static const int DX_ODD[6]  = { 1, 1, 0, -1, -1, 0 };
static const int DY_ODD[6]  = { 0, -1, -1, 0, 1, 1 };
//индекс клетки в одномерном масииве
int cell_index(int x, int y, int w) {
    return y * w + x;
}
//протвоп направление
int opposite_dir(int d) 
{
    return (d + 3) % 6;
}
//смешение чет/нечет
void neighbor_offset(int y, int d, int *dx, int *dy) {
    if (y % 2 == 0) {
        *dx = DX_EVEN[d];
        *dy = DY_EVEN[d];
    } else {
        *dx = DX_ODD[d];
        *dy = DY_ODD[d];
    }
}
//создание сетки
Cell *grid_alloc(int w, int h) {
    Cell *grid = (Cell *)calloc((size_t)w * (size_t)h, sizeof(Cell));
    return grid;
}
//очистка решетки
void grid_clear(Cell *grid, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        grid[i].state = 0;
        grid[i].is_obstacle = false;
    }
}
//заполнение клетки рандом частицами
void lattice_init(Cell *grid, int w, int h, double plot, unsigned int seed) {
    srand(seed);

    double p = plot / 7.0;
    if (p < 0.0) p = 0.0;
    if (p > 1.0) p = 1.0;

    for (int i = 0; i < w * h; i++) {
        grid[i].state = 0;
        grid[i].is_obstacle = false;

        for (int b = 0; b < 7; b++) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r < p) {
                grid[i].state |= (uint8_t)(1u << b);
            }
        }
    }
}
//цилиндр в центре сетки
void place_cylinder(Cell *grid, int w, int h, int diameter) {
    int cx = w / 2;
    int cy = h / 2;
    int radius = diameter / 2;

    if (radius <= 0) return;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int dx = x - cx;
            int dy = y - cy;
            int id = cell_index(x, y, w);

            if (dx * dx + dy * dy <= radius * radius) {
                grid[id].is_obstacle = true;
                grid[id].state = 0;
            } else {
                grid[id].is_obstacle = false;
            }
        }
    }
}
//движение частицц
void step_streaming(const Cell *src, Cell *dst, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        dst[i].state = 0;
        dst[i].is_obstacle = src[i].is_obstacle;
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int id = cell_index(x, y, w);
            uint8_t state = src[id].state & STATE_MASK;

            if (src[id].is_obstacle) {
                continue;
            }

            for (int d = 0; d < 6; d++) {
                if ((state & (1u << d)) == 0) {
                    continue;
                }

                int dx, dy;
                neighbor_offset(y, d, &dx, &dy);

                int nx = x + dx;
                int ny = y + dy;

                if (ny < 0 || ny >= h) {
                    dst[id].state |= (uint8_t)(1u << opposite_dir(d));
                    continue;
                }

                if (nx < 0 || nx >= w) {
                    continue;
                }

                int nid = cell_index(nx, ny, w);
                if (src[nid].is_obstacle) {
                    dst[id].state |= (uint8_t)(1u << opposite_dir(d));
                } else {
                    dst[nid].state |= (uint8_t)(1u << d);
                }
            }

            if (state & (1u << REST_BIT)) {
                dst[id].state |= (uint8_t)(1u << REST_BIT);
            }
        }
    }
}

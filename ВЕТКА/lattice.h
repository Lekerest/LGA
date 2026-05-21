#ifndef LATTICE_H
#define LATTICE_H

#include <stdint.h>
#include "cell.h"

int cell_index(int x, int y, int w);
int opposite_dir(int d);
void neighbor_offset(int y, int d, int *dx, int *dy);

Cell *grid_alloc(int w, int h);
void grid_clear(Cell *grid, int w, int h);
void lattice_init(Cell *grid, int w, int h, double plot, unsigned int seed);
void place_cylinder(Cell *grid, int w, int h, int diameter);

void step_streaming(const Cell *src, Cell *dst, int w, int h);

#endif

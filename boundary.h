#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "cell.h"

void apply_inlet(Cell *grid, int w, int h, double rho_in);
void apply_outlet(Cell *grid, int w, int h);

#endif

#ifndef COLLISION_H
#define COLLISION_H

#include <stdint.h>
#include "cell.h"

uint8_t apply_collision(uint8_t state);
void step_collision(Cell *grid, int w, int h);

#endif

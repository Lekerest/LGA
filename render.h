#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "cell.h"

double cell_density(const Cell *c);
double compute_avg_density(const Cell *grid, int w, int h);
void density_to_rgb(double plot, uint8_t *r, uint8_t *g, uint8_t *b);
void save_ppm(const Cell *grid, int w, int h, uint64_t iter, const char *dir);
void render_ansi(const Cell *grid, int w, int h, uint64_t iter, double plot_avg);
void render_frame(const Cell *grid, int w, int h, uint64_t iter, const char *dir);

#endif

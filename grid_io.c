#include <stdio.h> // файлы
#include <stdlib.h>
#include "grid_io.h"

void grid_save(const Cell *grid, uint32_t w, uint32_t h,
               uint64_t iter, double plot_in, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) 
    {
        perror("grid_save fopen");
        return;
    }

    GridHeader hdr;
    hdr.magic = GRID_MAGIC;
    hdr.width = w;
    hdr.height = h;
    hdr.iteration = iter;
    hdr.plot_in = plot_in;

    if (fwrite(&hdr, sizeof(hdr), 1, f) != 1) 
    {
        perror("grid_save header");
        fclose(f);
        return;
    }

    for (uint32_t i = 0; i < w * h; i++) 
    {
        uint8_t state = grid[i].state & STATE_MASK;
        if (fwrite(&state, sizeof(uint8_t), 1, f) != 1) 
        {
            perror("grid_save state");
            fclose(f);
            return;
        }
    }

    fclose(f);
    printf("[grid_io] Saved %s (iter %llu, %ux%u)\n",
           path, (unsigned long long)iter, w, h);
}

int grid_load(Cell *grid, uint32_t w, uint32_t h,
              uint64_t *iter, double *plot_in, const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) 
    {
        return 0;
    }

    GridHeader hdr;
    if (fread(&hdr, sizeof(hdr), 1, f) != 1) 
    {
        fprintf(stderr, "[grid_io] Cannot read header from %s\n", path);
        fclose(f);
        return 0;
    }

    if (hdr.magic != GRID_MAGIC) {
        fprintf(stderr, "[grid_io] Bad magic in %s\n", path);
        fclose(f);
        return 0;
    }

    if (hdr.width != w || hdr.height != h) 
    {
        fprintf(stderr, "[grid_io] Size mismatch: file %ux%u, expected %ux%u\n",
                hdr.width, hdr.height, w, h);
        fclose(f);
        return 0;
    }

    for (uint32_t i = 0; i < w * h; i++) 
    {
        uint8_t state = 0;
        if (fread(&state, sizeof(uint8_t), 1, f) != 1) 
        {
            fprintf(stderr, "[grid_io] Cannot read grid data from %s\n", path);
            fclose(f);
            return 0;
        }
        grid[i].state = state & STATE_MASK;
        grid[i].is_obstacle = false;
    }

    *iter = hdr.iteration;
    *plot_in = hdr.plot_in;

    fclose(f);
    printf("[grid_io] Resumed from %s (iter %llu, plot_in=%.3f)\n",
           path, (unsigned long long)*iter, *plot_in);
    return 1;
}

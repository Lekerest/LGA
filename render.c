#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

#include "render.h"
#include "lattice.h"

static int popcount7(uint8_t x) {
    int c = 0;
    x &= STATE_MASK;
    while (x) {
        c += x & 1u;
        x >>= 1;
    }
    return c;
}

static void make_dir(const char *dir) {
#ifdef _WIN32
    _mkdir(dir);
#else
    mkdir(dir, 0755);
#endif
}
//считаем плотность одной клетки
double cell_density(const Cell *c) {
    return (double)popcount7(c->state);
}
//средняя плотность сетки
double compute_avg_density(const Cell *grid, int w, int h) {
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < w * h; i++) {
        if (!grid[i].is_obstacle) {
            sum += cell_density(&grid[i]);
            count++;
        }
    }

    return count > 0 ? sum / (double)count : 0.0;
}
//плотность в цвет две интерполяции син зел и зел красный
void density_to_rgb(double plot, uint8_t *r, uint8_t *g, uint8_t *b) {
    double t = plot / 7.0;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    if (t < 0.5) {
        double s = t * 2.0;
        *r = (uint8_t)(0.0 + s * 30.0);
        *g = (uint8_t)(50.0 + s * 150.0);
        *b = (uint8_t)(200.0 - s * 150.0);
    } else {
        double s = (t - 0.5) * 2.0;
        *r = (uint8_t)(30.0 + s * 190.0);
        *g = (uint8_t)(200.0 - s * 170.0);
        *b = (uint8_t)(50.0 - s * 20.0);
    }
}
//сохраниеие кадра
void save_ppm(const Cell *grid, int w, int h, uint64_t iter, const char *dir) {
    make_dir(dir);

    char path[256];
    snprintf(path, sizeof(path), "%s/frame_%06llu.ppm", dir, (unsigned long long)iter);

    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("save_ppm fopen");
        return;
    }

    fprintf(f, "P6\n%d %d\n255\n", w, h);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            const Cell *c = &grid[cell_index(x, y, w)];
            uint8_t r, g, b;

            if (c->is_obstacle) {
                r = 60;
                g = 60;
                b = 60;
            } else {
                density_to_rgb(cell_density(c), &r, &g, &b);
            }

            fwrite(&r, 1, 1, f);
            fwrite(&g, 1, 1, f);
            fwrite(&b, 1, 1, f);
        }
    }

    fclose(f);
}
//красим терминал в нужные цвет
void render_ansi(const Cell *grid, int w, int h, uint64_t iter, double plot_avg) {
    if (w > 80) return;

    printf("\033[2J\033[H");
    printf("iter: %llu | plot avg: %.4f | size: %dx%d\n", (unsigned long long)iter, plot_avg, w, h);

    printf("+");
    for (int x = 0; x < w; x++) printf("--");
    printf("+\n");

    for (int y = 0; y < h; y++) {
        printf("|");
        for (int x = 0; x < w; x++) {
            const Cell *c = &grid[cell_index(x, y, w)];
            uint8_t r, g, b;

            if (c->is_obstacle) {
                r = 80;
                g = 80;
                b = 80;
            } else {
                density_to_rgb(cell_density(c), &r, &g, &b);
            }

            printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < w; x++) printf("--");
    printf("+\n");
}
//сохраняем кадлые н инетераций кадр и выводим анси
void render_frame(const Cell *grid, int w, int h, uint64_t iter, const char *dir) {
    double plot_avg = compute_avg_density(grid, w, h);
    save_ppm(grid, w, h, iter, dir);

    if (w <= 80) {
        render_ansi(grid, w, h, iter, plot_avg);
    }
}

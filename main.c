#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "cell.h"
#include "lattice.h"
#include "collision.h"
#include "boundary.h"
#include "grid_io.h"
#include "render.h"

static void print_help(const char *prog) {
    printf("Usage:\n");
    printf("  %s [options]\n\n", prog);
    printf("Options:\n");
    printf("  -w <int>           grid width, default 256\n");
    printf("  -h <int>           grid height, default 160\n");
    printf("  -n <int>           iterations, default 1000\n");
    printf("  -d <int>           cylinder diameter, default 30\n");
    printf("  -v <int>           visualization interval, default 100\n");
    printf("  -s <int>           random seed\n");
    printf("  --plot-in <double>  inlet density, default 1.6\n");
    printf("  --plot-init <dbl>   initial density, default 1.0\n");
    printf("  --resume           load grid.bin and continue\n");
    printf("  --bin <path>       binary state path, default grid.bin\n");
    printf("  --frames <dir>     PPM output directory, default frames\n");
    printf("  --help             show help\n");
}

static int need_value(int i, int argc, const char *opt) {
    if (i + 1 >= argc) {
        fprintf(stderr, "Option %s requires value\n", opt);
        exit(1);
    }
    return i + 1;
}

static int parse_int(const char *s, const char *name) {
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (!end || *end != '\0') {
        fprintf(stderr, "Bad integer for %s: %s\n", name, s);
        exit(1);
    }
    return (int)v;
}

static double parse_double(const char *s, const char *name) {
    char *end = NULL;
    double v = strtod(s, &end);
    if (!end || *end != '\0') {
        fprintf(stderr, "Bad double for %s: %s\n", name, s);
        exit(1);
    }
    return v;
}

static void params_default(SimParams *p) {
    p->width = 256;
    p->height = 160;
    p->iters = 1000;
    p->viz_interval = 100;
    p->diameter = 30;
    p->seed = (unsigned int)time(NULL);
    p->plot_in = 1.6;
    p->plot_init = 1.0;
    p->resume = false;
    p->bin_path = "grid.bin";
    p->frames_dir = "frames";
}

static void parse_args(int argc, char **argv, SimParams *p) {
    params_default(p);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0) {
            i = need_value(i, argc, "-w");
            p->width = parse_int(argv[i], "width");
        } else if (strcmp(argv[i], "-h") == 0) {
            i = need_value(i, argc, "-h");
            p->height = parse_int(argv[i], "height");
        } else if (strcmp(argv[i], "-n") == 0) {
            i = need_value(i, argc, "-n");
            p->iters = (uint64_t)parse_int(argv[i], "iters");
        } else if (strcmp(argv[i], "-d") == 0) {
            i = need_value(i, argc, "-d");
            p->diameter = parse_int(argv[i], "diameter");
        } else if (strcmp(argv[i], "-v") == 0) {
            i = need_value(i, argc, "-v");
            p->viz_interval = parse_int(argv[i], "viz_interval");
        } else if (strcmp(argv[i], "-s") == 0) {
            i = need_value(i, argc, "-s");
            p->seed = (unsigned int)parse_int(argv[i], "seed");
        } else if (strcmp(argv[i], "--plot-in") == 0) {
            i = need_value(i, argc, "--plot-in");
            p->plot_in = parse_double(argv[i], "plot_in");
        } else if (strcmp(argv[i], "--plot-init") == 0) {
            i = need_value(i, argc, "--plot-init");
            p->plot_init = parse_double(argv[i], "plot_init");
        } else if (strcmp(argv[i], "--resume") == 0) {
            p->resume = true;
        } else if (strcmp(argv[i], "--bin") == 0) {
            i = need_value(i, argc, "--bin");
            p->bin_path = argv[i];
        } else if (strcmp(argv[i], "--frames") == 0) {
            i = need_value(i, argc, "--frames");
            p->frames_dir = argv[i];
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            exit(0);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            exit(1);
        }
    }

    if (p->width < 8 || p->height < 8) {
        fprintf(stderr, "Grid is too small\n");
        exit(1);
    }
    if (p->viz_interval <= 0) {
        fprintf(stderr, "viz_interval must be positive\n");
        exit(1);
    }
    if (p->diameter < 0 || p->diameter >= p->height || p->diameter >= p->width) {
        fprintf(stderr, "Bad cylinder diameter\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    SimParams params;
    parse_args(argc, argv, &params);

    srand(params.seed);

    Cell *grid = grid_alloc(params.width, params.height);
    Cell *next = grid_alloc(params.width, params.height);

    uint64_t start_iter = 0;

    if (params.resume) {
        if (!grid_load(grid, (uint32_t)params.width, (uint32_t)params.height,
                       &start_iter, &params.plot_in, params.bin_path)) {
            fprintf(stderr, "Warning: cannot load %s, starting fresh\n", params.bin_path);
            lattice_init(grid, params.width, params.height, params.plot_init, params.seed);
            start_iter = 0;
        }
        place_cylinder(grid, params.width, params.height, params.diameter);
    } else {
        lattice_init(grid, params.width, params.height, params.plot_init, params.seed);
        place_cylinder(grid, params.width, params.height, params.diameter);
    }

    printf("LGA simulation started\n");
    printf("size=%dx%d, iters=%llu, plot_in=%.3f, diameter=%d, seed=%u\n",
           params.width, params.height, (unsigned long long)params.iters,
           params.plot_in, params.diameter, params.seed);

    uint64_t end_iter = start_iter + params.iters;

    for (uint64_t iter = start_iter + 1; iter <= end_iter; iter++) {
        step_collision(grid, params.width, params.height);
        step_streaming(grid, next, params.width, params.height);
        apply_inlet(next, params.width, params.height, params.plot_in);
        apply_outlet(next, params.width, params.height);

        Cell *tmp = grid;
        grid = next;
        next = tmp;

        if (iter % (uint64_t)params.viz_interval == 0) {
            render_frame(grid, params.width, params.height, iter, params.frames_dir);
        }
    }

    double plot_avg = compute_avg_density(grid, params.width, params.height);
    printf("\nFinal result\n");
    printf("iteration: %llu\n", (unsigned long long)end_iter);
    printf("plot avg:   %.6f\n", plot_avg);

    grid_save(grid, (uint32_t)params.width, (uint32_t)params.height,
              end_iter, params.plot_in, params.bin_path);

    free(grid);
    free(next);

    return 0;
}

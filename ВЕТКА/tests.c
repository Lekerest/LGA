#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cell.h"
#include "lattice.h"
#include "collision.h"
#include "grid_io.h"
#include "render.h"

static int failures = 0;

static void check_int(const char *name, int got, int expected) {
    if (got != expected) {
        printf("[FAIL] %s: got %d, expected %d\n", name, got, expected);
        failures++;
    } else {
        printf("[ OK ] %s\n", name);
    }
}

static void check_true(const char *name, int ok) {
    if (!ok) {
        printf("[FAIL] %s\n", name);
        failures++;
    } else {
        printf("[ OK ] %s\n", name);
    }
}

static void test_opposite_dir(void) {
    check_int("opposite 0", opposite_dir(0), 3);
    check_int("opposite 1", opposite_dir(1), 4);
    check_int("opposite 2", opposite_dir(2), 5);
    check_int("opposite 3", opposite_dir(3), 0);
}

static void test_collision_rules(void) {
    srand(1);
    uint8_t out = apply_collision((1u << 0) | (1u << 3));
    check_true("head-on collision keeps two particles", cell_density(&(Cell){ .state = out }) == 2.0);

    out = apply_collision((1u << 0) | (1u << 2) | (1u << 4));
    check_int("three-particle collision", out, (1u << 1) | (1u << 3) | (1u << 5));
}

static void test_grid_io(void) {
    int w = 8, h = 6;
    Cell *grid = grid_alloc(w, h);
    Cell *loaded = grid_alloc(w, h);

    grid[0].state = 7;
    grid[10].state = 64;
    grid[12].state = 31;

    grid_save(grid, (uint32_t)w, (uint32_t)h, 123, 1.75, "test_grid.bin");

    uint64_t iter = 0;
    double plot = 0.0;
    int ok = grid_load(loaded, (uint32_t)w, (uint32_t)h, &iter, &plot, "test_grid.bin");

    check_true("grid_load ok", ok == 1);
    check_true("grid_load iter", iter == 123);
    check_true("grid_load plot", fabs(plot - 1.75) < 1e-9);
    check_int("grid_load state", loaded[12].state, 31);

    free(grid);
    free(loaded);
    remove("test_grid.bin");
}

static void test_streaming_wall_bounce(void) {
    int w = 10, h = 10;
    Cell *grid = grid_alloc(w, h);
    Cell *next = grid_alloc(w, h);

    int id = cell_index(4, 0, w);
    grid[id].state = (1u << DIR_NE);
    step_streaming(grid, next, w, h);

    check_true("top wall bounce", (next[id].state & (1u << DIR_SW)) != 0);

    free(grid);
    free(next);
}

static void test_avg_density(void) {
    int w = 4, h = 4;
    Cell *grid = grid_alloc(w, h);
    grid[0].state = 3;
    grid[1].state = 1;
    double plot = compute_avg_density(grid, w, h);
    check_true("avg density", fabs(plot - 3.0 / 16.0) < 1e-9);
    free(grid);
}

int main(void) {
    test_opposite_dir();
    test_collision_rules();
    test_grid_io();
    test_streaming_wall_bounce();
    test_avg_density();

    if (failures == 0) {
        printf("\nAll tests passed.\n");
        return 0;
    }

    printf("\nTests failed: %d\n", failures);
    return 1;
}

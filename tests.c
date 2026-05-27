#include <stdio.h> // вывод в текста в консоль
#include <stdlib.h> // srand free remove
#include <math.h> // для модуля разницы

#include "cell.h"
#include "lattice.h"
#include "collision.h"
#include "grid_io.h"
#include "render.h"

static int failures = 0;

static void check_int(const char *name, int got, int expected)  // имя + фр + ор
{
    if (got != expected) 
    {
        printf("[FAIL] %s: got %d, expected %d\n", name, got, expected);
        failures++;
    } 
    else 
    {
        printf("[ OK ] %s\n", name);
    }
}

static void check_true(const char *name, int ok) 
{
    if (!ok) 
    {
        printf("[FAIL] %s\n", name);
        failures++;
    } 
    else 
    {
        printf("[ OK ] %s\n", name);
    }
}

static void test_opposite_dir(void) // проверяем что разворот поворот нормис
{
    check_int("opposite 0", opposite_dir(0), 3); // opposite_dir из lattice.c
    check_int("opposite 1", opposite_dir(1), 4); // (d + 3) % 6
    check_int("opposite 2", opposite_dir(2), 5); // 0 - вправо, 1 - вверх вправо, 2 - вверх влево, 3 - влево, 4 - вниз влево, 5 - вниз вправо
    check_int("opposite 3", opposite_dir(3), 0);
}

static void test_collision_rules(void)  // проверяем что 2 друна находят норм решение конфликта
{
    srand(1); // ставим 0 и 3 в 1 клетку они такие типа ватафак куда кому пройти и apply_collision из collision.c решает это
    uint8_t out = apply_collision((1u << 0) | (1u << 3)); // если 0 и 3 то может быть как 1 4 так и 2 5 поэтому проверяем что друны остались вдвоем а не потерялись
    check_true("head-on collision keeps two particles", cell_density(&(Cell){ .state = out }) == 2.0);

    out = apply_collision((1u << 0) | (1u << 2) | (1u << 4)); // тут друнов уже 3 и по правилам 3-particle collision. 0 2 4 переходит в 1 3 5 и наоборот
    check_int("three-particle collision", out, (1u << 1) | (1u << 3) | (1u << 5));
}

static void test_grid_io(void) // чекаем что сохранение и загрузка карамелек работает
{ // сохранить состояние решётки в файл -> загрузить его обратно -> восстановить номер итерации -> восстановить rho_in -> восстановить state клеток
    int w = 8, h = 6; // мини решетка для теста 8 ширина 6 высота
    Cell *grid = grid_alloc(w, h); // grid_alloc из lattice.c
    Cell *loaded = grid_alloc(w, h); // grid начальная для сохранения в файл loaded потом вторая для загрузки из файла

    grid[0].state = 7; // 0000111
    grid[10].state = 64; // 1000000
    grid[12].state = 31; // 0011111

    grid_save(grid, (uint32_t)w, (uint32_t)h, 123, 1.75, "test_grid.bin"); // grid_save из grid_io.c

    uint64_t iter = 0;
    double rho_loaded = 0.0;
    int ok = grid_load(loaded, (uint32_t)w, (uint32_t)h, &iter, &rho_loaded, "test_grid.bin"); // grid_load из grid_io.c

    check_true("grid_load ok", ok == 1);
    check_true("grid_load iter", iter == 123);
    check_true("grid_load rho_loaded", fabs(rho_loaded - 1.75) < 1e-9); // из за формата хранения числа в double может быть микро разница
    check_int("grid_load state", loaded[12].state, 31);                 //  поэтому сравниваем что < 1e-9 эта разница и тогда типа равно

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

int main(void) 
{
    test_opposite_dir();
    test_collision_rules();
    test_grid_io();
    test_streaming_wall_bounce();
    test_avg_density();

    if (failures == 0) 
    {
        printf("\nAll tests passed.\n");
        return 0;
    }

    printf("\nTests failed: %d\n", failures);
    return 1;
}

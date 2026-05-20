#include <stdlib.h>
#include "collision.h"

static int has_bit(uint8_t s, int b) {
    return (s & (1u << b)) != 0;
}

static int moving_count(uint8_t s) {
    int c = 0;
    for (int d = 0; d < 6; d++) {
        if (has_bit(s, d)) c++;
    }
    return c;
}

uint8_t apply_collision(uint8_t state) {
    state &= STATE_MASK;

    uint8_t rest = state & (1u << REST_BIT);
    uint8_t moving = state & 0x3F;

    /* 2-particle head-on collisions. */
    if (!rest && moving_count(state) == 2) {
        if (moving == ((1u << 0) | (1u << 3))) {
            return (rand() & 1) ? (uint8_t)((1u << 1) | (1u << 4))
                                : (uint8_t)((1u << 2) | (1u << 5));
        }
        if (moving == ((1u << 1) | (1u << 4))) {
            return (rand() & 1) ? (uint8_t)((1u << 0) | (1u << 3))
                                : (uint8_t)((1u << 2) | (1u << 5));
        }
        if (moving == ((1u << 2) | (1u << 5))) {
            return (rand() & 1) ? (uint8_t)((1u << 0) | (1u << 3))
                                : (uint8_t)((1u << 1) | (1u << 4));
        }
    }

    /* 3-particle symmetric collisions. */
    if (!rest && moving == ((1u << 0) | (1u << 2) | (1u << 4))) {
        return (uint8_t)((1u << 1) | (1u << 3) | (1u << 5));
    }
    if (!rest && moving == ((1u << 1) | (1u << 3) | (1u << 5))) {
        return (uint8_t)((1u << 0) | (1u << 2) | (1u << 4));
    }

    /* Simple rest-particle case: opposite pair + rest -> symmetric triplet. */
    if (rest && moving_count(state) == 2) {
        int opposite_pair =
            moving == ((1u << 0) | (1u << 3)) ||
            moving == ((1u << 1) | (1u << 4)) ||
            moving == ((1u << 2) | (1u << 5));

        if (opposite_pair) {
            return (rand() & 1) ? (uint8_t)((1u << 0) | (1u << 2) | (1u << 4))
                                : (uint8_t)((1u << 1) | (1u << 3) | (1u << 5));
        }
    }

    return state;
}

void step_collision(Cell *grid, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        if (!grid[i].is_obstacle) {
            grid[i].state = apply_collision(grid[i].state);
        }
    }
}

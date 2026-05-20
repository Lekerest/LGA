#!/bin/bash
set -e

CC=${CC:-clang}
OUT=tests
SRC="tests.c lattice.c collision.c boundary.c grid_io.c render.c"

if ! command -v "$CC" >/dev/null 2>&1; then
    echo "TEST BUILD ERROR: compiler '$CC' not found"
    echo "Install Xcode Command Line Tools: xcode-select --install"
    exit 1
fi

echo "Compiler: $($CC --version | head -n 1)"
$CC -O2 -Wall -Wextra -std=c11 $SRC -o "$OUT" -lm

echo "TEST BUILD OK"
./$OUT

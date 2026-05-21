#!/bin/bash
set -e

if [ ! -x ./lga ]; then
    echo "lga not found, building first..."
    ./build_mac.sh
fi

./lga -w 80 -h 80 -n 1000 -d 10 --plot-in 1.6 -v 1

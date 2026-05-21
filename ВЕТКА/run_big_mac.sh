#!/bin/bash
set -e

if [ ! -x ./lga ]; then
    echo "lga not found, building first..."
    ./build_mac.sh
fi

./lga -w 256 -h 160 -n 1000 -d 30 --rho-in 1.8 -v 100

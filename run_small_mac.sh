#!/bin/bash
set -e

if [ ! -x ./lga ]; then
    echo "lga not found, building first..."
    ./build_mac.sh
fi

./lga -w 60 -h 40 -n 500 -d 10 --rho-in 1.6 -v 10

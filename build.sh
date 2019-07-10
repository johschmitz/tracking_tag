#!/bin/bash

# Usage help
display_usage() {
    echo -e "Usage: ./build.sh ID"
}
if [[ $1 == "--help" ||  $1 == "-h" ]]
    then
        display_usage
    exit 0
fi
if [[ $# -le 0 || $# -ge 2 ]]
    then
        display_usage
    exit 1
fi

ID=$1

make clean
mkdir -p build
cd build
../python/generate_cdma_code_sequence.py -i $ID -l 8192 -r 0.999756e6 -o 2e6 --fft-input-padding
cp cdma_sequence.h ../include/
cd ..
make

exit 0

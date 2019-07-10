#!/bin/bash

NUM_TAGS=$1
DIR_OUTPUT="build_multi"

display_usage() {
    echo "Usage: ./build_multi.sh [NUMBER_OF_TAGS]"
}
if [[ ( $1 == "--help") ||  $1 == "-h" ]] 
    then
        display_usage
    exit 0
fi

mkdir -p $DIR_OUTPUT/reference/
mkdir -p $DIR_OUTPUT/firmware/
for i in `seq 0 $((NUM_TAGS-1))`;
do
    ID="000$i"
    ID=${ID:(-4)}
    echo
    echo "ID: $ID"
    ./build.sh $ID
    cp build/cdma_sequence_fft_id_$ID.complex $DIR_OUTPUT/reference/
    cp build/main.ihx $DIR_OUTPUT/firmware/$ID.ihx
done

#!/usr/bin/env bash

max=100
for i in `seq 0 $max`
do
    echo "$i"
    ./noise_generator.o 512 512 noise-$i.bmp
done

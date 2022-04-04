#!/bin/bash

numbers=8;

# compile cpp src
mpic++ --prefix /usr/local/share/OpenMPI -o oems oems.cpp

# create file with random numbers
dd if=/dev/random bs=1 count=$numbers of=numbers

# run
mpirun --prefix /usr/local/share/OpenMPI -np $numbers oems

# clean
rm -f oems numbers
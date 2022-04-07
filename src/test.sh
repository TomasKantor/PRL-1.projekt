#!/bin/bash

numbers=8;
processors=19;

# compile cpp src
mpicc --prefix /usr/local/share/OpenMPI -o oems oems.c

# create file with random numbers
dd if=/dev/random bs=1 count=$numbers of=numbers 2> /dev/null

# run
mpirun --prefix /usr/local/share/OpenMPI --oversubscribe -np $processors oems

# clean
rm -f oems numbers
#!/bin/sh
clear
mpicc base_mpi.c -o base_mpi
qsub qsubMPI.qsub
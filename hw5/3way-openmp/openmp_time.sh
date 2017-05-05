clear

rm openmp_time
gcc openmp_time.c base.c -o openmp_time -fopenmp -fbounds-check

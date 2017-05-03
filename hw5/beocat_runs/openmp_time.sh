clear

rm openmp_time
gcc openmp_time.c openmp-sol.c -o openmp_time -fopenmp -fbounds-check

clear

rm pthreads_time
gcc pthreads_time.c pthreads.c -o pthreads_time -lpthread -fbounds-check

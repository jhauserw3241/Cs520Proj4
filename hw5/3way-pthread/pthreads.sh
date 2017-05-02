clear

rm pthreads
gcc pthreads.c -o pthreads -lpthread -fbounds-check

./pthreads ../tests/input_files/test-long-src.txt ../tests/input_files/test-long-in.txt

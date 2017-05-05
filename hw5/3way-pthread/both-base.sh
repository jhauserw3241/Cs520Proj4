clear

rm both-base
gcc both-base.c -o both-base -lpthread -fbounds-check

./both-base ../tests/input_files/test-long-src.txt ../tests/input_files/test-long-in.txt

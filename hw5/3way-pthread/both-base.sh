clear

rm both-base
gcc both-base.c -o both-base -lpthread -fbounds-check

./both-base ../test-long-src.txt ../test-long-in.txt

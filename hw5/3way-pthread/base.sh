clear

rm base
gcc base.c -o base -lpthread -fbounds-check

./base ../test-long-src.txt ../test-long-in.txt

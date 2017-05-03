clear

rm serial-base
gcc serial-base.c -o serial-base -lpthread

./serial-base ../lorem-ipsum-src.txt ../lorem-ipsum-in.txt

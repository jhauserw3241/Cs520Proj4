clear

rm base
gcc base.c -o base -lpthread

./base ../lorem-ipsum-src.txt ../lorem-ipsum-in.txt

clear

rm base
gcc base.c -o base -lpthread -fbounds-check

./base ../lorem-ipsum-src.txt ../lorem-ipsum-in.txt

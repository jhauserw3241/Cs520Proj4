clear

rm base
gcc base.c -o base -lpthread

./base source.txt input.txt

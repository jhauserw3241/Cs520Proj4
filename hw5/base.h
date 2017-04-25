#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 50000
#define STRING_SIZE 300

char source_array[ARRAY_SIZE][STRING_SIZE];
char input_array[ARRAY_SIZE][STRING_SIZE];
char output_array[ARRAY_SIZE][STRING_SIZE];

int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int SearchForTerm(char term[], int start, int end);

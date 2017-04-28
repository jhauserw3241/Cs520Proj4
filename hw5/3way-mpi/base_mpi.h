#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 50000
#define STRING_SIZE 300
#define COUNT 2 //number of terms in source divided by number of mpi cores used

char source_array[ARRAY_SIZE][STRING_SIZE];
char input_array[ARRAY_SIZE][STRING_SIZE];
char output_array[ARRAY_SIZE][STRING_SIZE];
int counter = 0, in_counter = 0;
char sub_out[COUNT][STRING_SIZE];
	
int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int SearchForTerm(char term[], int source_index, int start, int end);
void PrintResults();

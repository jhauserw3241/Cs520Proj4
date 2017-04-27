#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 50000
#define STRING_SIZE 300

char source_array[ARRAY_SIZE][STRING_SIZE];
char input_array[ARRAY_SIZE][STRING_SIZE];
char output_array[ARRAY_SIZE][STRING_SIZE];

typedef struct arg{
	int chunk_ID;
	char *term;
	int diff;
	int source_index;
}arg_t;

int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
void *SearchForTerm(void *args);
void PrintResults();

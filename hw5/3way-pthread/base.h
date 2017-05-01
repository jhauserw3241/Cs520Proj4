#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arg{
	char *term;
	int diff;
	int start;
	int end;
	int source_index;
}arg_t;

typedef struct output_info {
	int count;
	int size;
} out_info;

int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int ReadSourceData(char *filename);
void *SearchForTerm(void *args);
void PrintResults();


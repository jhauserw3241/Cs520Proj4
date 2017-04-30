#include <stdio.h>
#include <stdlib.h>

typedef struct arg{
	char *term;
	int diff;
	int start;
	int end;
	int source_index;
}arg_t;

int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int ReadSourceData(char *filename);
void *SearchForTerm(void *args);
void PrintResults();

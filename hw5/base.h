#include <stdio.h>
#include <stdlib.h>

char *source_array[100];
char *input_array[100];

int main(int argc, char *argv[]);
int InitializeArraysFromFiles(char *file_name, char *array[]);
void FreeArrayElements(char **array);
void CompareElements(char **source, char **input, char **output);

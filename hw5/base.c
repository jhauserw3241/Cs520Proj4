#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#define ARRAY_SIZE 50000
#define STRING_SIZE 300

int main(int argc, char *argv[])
{	
	
	char *source_array[ARRAY_SIZE];
	char *input_array[ARRAY_SIZE];
	char *output[ARRAY_SIZE];
	
	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}

	// Read files into arrays
	if(InitializeArraysFromFiles(argv[1], source_array) != 0) {
		printf("Source file could not be read\n");
	}

	if(InitializeArraysFromFiles(argv[2], input_array) != 0) {
		printf("Input file could not be read\n");
	}

	// Find matching entries
	CompareElements(source_array, input_array, output);

	FreeArrayElements(source_array);
	FreeArrayElements(input_array);
	FreeArrayElements(output);

	return 0;
}

int InitializeArraysFromFiles(char *file_name, char *array[]) {
	FILE *f;
	char *c;
	int i = 0, j = 0;
	size_t len = 0;
	ssize_t read;
	char *temp;
	
	f = fopen(file_name, "rt");
	if (f == NULL)
		return -1;

	temp = malloc(STRING_SIZE);
	while((c = fgetc(f)) != EOF) {
		if(c == '\n') {
			array[i] = temp;
			temp = malloc(STRING_SIZE);
			i++;
			j = 0;
		}
		else {
			temp[j] = c;
			j++;
		}
	}
	
	fclose(f);

	// Set last element to null so you don't need to keep track
	// of the number of elements in the array
	temp = malloc(STRING_SIZE);
	temp[j] = NULL;
	array[i] = temp;

	return 0;
}

void FreeArrayElements(char **array) {
	int i;

	for(i = 0; array[i] != NULL; i++) {
		free(array[i]);
		i++;
	}

	// Free NULL element at end
	free(array[i]);
}

void CompareElements(char **source, char **input, char **output) {
	int temp[ARRAY_SIZE];
	int *tempNum;
	int t = 0;

	for(int i = 0; source[i] != NULL; i++) {
		if(source[i][0] != '\0') {
			for(int j = 0; input[j] != NULL; j++) {
				if((input[j][0] != '\0') && (strstr(input[j], source[i]))) {
					temp[t] = j + 1;
					t++;
				}
			}
		}

		printf("%s ", source[i]);
		for(int k = 0; k < t; k++) {
			if((k == (t - 1)) && (t != 1)) {
				printf("%d, ", temp[k]);
			}
			else {
				printf("%d\n", temp[k]);
			}
		}
		t = 0;
	}
}

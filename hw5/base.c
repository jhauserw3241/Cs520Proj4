#include <stdio.h>
#include <stdlib.h>

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
	return 0;
}

void FreeArrayElements(char *array[]) {
	int size = sizeof(array)/STRING_SIZE;
	for(int i = 0; i < size; i++) {
		free(array[i]);
	}
}

void CompareElements(char *source, char *input, char *output) {
	printf("%c\n", source[0]);
	printf("%c\n", source[1]);
	printf("%c\n", source[2]);

	int sourceSize = sizeof(source)/STRING_SIZE;
	int inputSize = sizeof(input)/STRING_SIZE;

	printf("Source Size: %d\n", sourceSize);
	printf("Input Size: %d\n", inputSize);

	for(int i = 0; i < sourceSize; i++) {
		printf("%s\n", source[i]);
	}

	for(int j = 0; j < inputSize; j++) {
		printf("%s\n", input[j]);
	}

	/*for(int i = 0; i < sourceSize; i++) {
		printf("%c\n", source[i]);
		for(int j = 0; j < inputSize; j++) {
			printf("%c\n", input[j]);
			printf("Hello\n");
			if(strstr(source[i], input[j]) != NULL) {
				printf("    Interrupt\n");
			}
		}
	}*/
}

/*int[][] CountArray(char **source, char**input) {
	char *source;
	int i, j, x = 0;

	int source_counts[source.length][ARRAY_SIZE];
	for(i = 0; i < source.length; i++) {
		source = source[i];
		for(j = 0; j < input.length; j++) {
			if(strstr(input[j], source) != NULL) {
				source_counts[i][x] = j;
				x++;
			}
		}
	}
}

void print_results(char **source, char **input) {
	char *word;
	int i, j = 0;
	int source_counts[source.length][ARRAY_SIZE] = CountArray(source, input);

	for(i = 0; i < sources.length; i++) {
		source = sources[i];
		printf("Instances of %s: ", source);
		for(j = 0; j < source_counts[i].length; j++) {
			if(source_counts[i][j] != NULL) {
				printf("%d, ", source_counts[i][j]);
			}
		}
	}
}*/

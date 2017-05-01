#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#define ARRAY_SIZE 2
#define STRING_SIZE 500
#define CHUNK_SIZE 3

int SourceArraySize = ARRAY_SIZE;
int InputArraySize = ARRAY_SIZE;
int OutputArraySize = ARRAY_SIZE;

int StringSize = STRING_SIZE;

char **source_array;
int source_count = 0;
char **input_array;
int input_count = 0;
char **output_array;

int main(int argc, char *argv[])
{	
	FILE *f;
	int i = 0;

	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}

	// Initialize source array
	source_array = malloc(sizeof(char) * SourceArraySize);

	f = fopen(argv[1], "rt");
	if(f == NULL)
		return -1;

	char *tempTerm = malloc(StringSize);
	while(fgets(tempTerm, StringSize, f) != NULL) {
		if(i >= SourceArraySize) {
			printf("Resize source array\n");
			char **temp = (char **)calloc(SourceArraySize * 2, sizeof(char*));
			memcpy(temp, source_array, sizeof(char*) * SourceArraySize);
			free(source_array);
			source_array = temp;
			SourceArraySize *= 2;
		}

		char *term = malloc(StringSize);
		memcpy(term, tempTerm, strlen(tempTerm));
		term[strlen(term) - 1] = 0;
		source_array[i] = term;
		i++;
		source_count++;
	}

	//free(term);

	fclose(f);

	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}

	PrintResults();
	printf("Finish printing results\n");

	return 0;
}

int SearchForTerm(char term[], int source_index, int start, int end) {
	int j = 0;

	for(int i = start; (i < input_count) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			char *temp = malloc(sizeof(char) * StringSize);

			if(output_array[source_index] || (j != 0)) {
				sprintf(temp, "%s, %i", output_array[source_index], i + 1);
			}
			else {
				sprintf(temp, "%i", i + 1);
			}

			output_array[source_index] = temp;
			j++;
		}
	}
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;

	int i = 0;
	int count = 0;
	int terms = sizeof(source_array) / sizeof(char **);
	
	// Initialize output array
	output_array = (char **)calloc(SourceArraySize, sizeof(char));

	// Initialize input array
	input_array = malloc(sizeof(char) * InputArraySize);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	char *tempLine = malloc(sizeof(char) * StringSize);
	while(fgets(tempLine, StringSize, f) != NULL) {
		if(i >= InputArraySize) {
			printf("Resize input array\n");
			char **temp = (char **)calloc(InputArraySize * 2, sizeof(char *));
			memcpy(temp, input_array, sizeof(char *) * InputArraySize);
			free(input_array);
			input_array = temp;
			InputArraySize *= 2;
		}

		char *line = malloc(sizeof(char) * StringSize);
		memcpy(line, tempLine, strlen(tempLine));
		line[strlen(line) - 1] = 0;
		input_array[i] = line;


		if((count + 1) == CHUNK_SIZE) {
			for(int j = 0; j < source_count; j++) {
				SearchForTerm(source_array[j], j, (i + 1) - CHUNK_SIZE, (i + 1));
			}
			count = 0;
		}
		else {
			count++;
		}

		i++;
		input_count++;
	}

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		for(int j = 0; j < source_count; j++) {
			SearchForTerm(source_array[j], j, i - dif, i);
		}
	}

	fclose(f);

	return 0;
}

void PrintResults() {
	for(int i = 0; i < source_count; i++) {
		printf("Before value check\n");
		printf("Output value: %s\n", output_array[i]);
		if(output_array[i]) {
			printf("%s %s\n", source_array[i], output_array[i]);
		}
		else {
			printf("No output\n");
		}
	}
}

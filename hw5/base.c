#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#define CHUNK_SIZE 100

int main(int argc, char *argv[])
{	
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0;

	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}

	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}

	f = fopen(argv[1], "rt");
	if(f == NULL)
		return -1;

	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(source_array[i], t);
		SearchForTerm(source_array[i], i, 0, 100);
		i++;
	}

	fclose(f);

	// Make the source list null terminated so that you don't need to find the size
	source_array[i][0] = NULL;

	PrintResults();

	return 0;
}

int SearchForTerm(char term[], int source_index, int start, int end) {
	int j = 0;

	for(int i = start; (input_array[i][0]) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			if((output_array[source_index][0] != '\0') || (j != 0)) {
				sprintf(output_array[source_index], "%s, %i", output_array[source_index], i + 1);
			}
			else {
				sprintf(output_array[source_index], "%i", i + 1);
			}
			j++;
		}
	}
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0;

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(input_array[i], t);
		i++;
	}

	// Add NULL to end so we don't need to keep track of the array size
	input_array[i][0] = NULL;

	fclose(f);

	return 0;
}

void PrintResults() {
	for(int i = 0; source_array[i][0]; i++) {
		printf("%s %s\n", source_array[i], output_array[i]);
	}
}

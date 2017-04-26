#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#define CHUNK_SIZE 3
#define TERMS 3

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

	// Read in source data
	f = fopen(argv[1], "rt");
	if(f == NULL)
		return -1;

	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(source_array[i], t);
		//SearchForTerm(source_array[i], 0, 100);
		i++;
	}
	for(int z = 0; z < 3; z++)
	{
		printf("%s\n", source_array[z]);
	}
	
	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}
	
	for(int z = 0; z < 10; z++)
	{
		printf("%s\n", input_array[z]);
	}


	fclose(f);

	return 0;
}

int SearchForTerm(char term[], int start, int end) {
	char temp[STRING_SIZE];

	// Set first element to NULL to check if the string has been started
	temp[0] = NULL;

	printf("%s", term);
	for(int i = start; (input_array[i][0]) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			printf(" %d", i + 1);

			if(temp[0]) {
				temp[i] = i + 1;
			}
			else {
				temp[i] = i + 1;
			}
		}
		printf("%s\n", temp);
	}
	printf("\n");
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0, count = 0;

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	printf("In read input \n");
	while(fgets(temp, STRING_SIZE, f) != NULL) {
		//char *t = temp;
		temp[strlen(temp) - 1] = 0;
		strcpy(input_array[i], temp);
		if (count == CHUNK_SIZE){
			for(int j = 0; j < TERMS; j++){
				//spawn of threads for each term and call searchterm
				SearchForTerm(source_array[j], i-CHUNK_SIZE, i);
			}
		}
		i++;
		count++;
		if (count > CHUNK_SIZE)
			count = 0;
	}

	// Add NULL to end so we don't need to keep track of the array size
	input_array[i][0] = NULL;

	fclose(f);

	return 0;
}

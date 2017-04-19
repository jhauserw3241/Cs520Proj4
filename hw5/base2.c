#include <stdio.h>
#include <stdlib.h>

#include "base2.h"


int main(int argc, char *argv[])
{	
	
	char *source_array[100];
	char *input_array[100];
	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}

	for(int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	// Read files into arrays
	if(InitializeArraysFromFiles(argv[1], source_array) != 0) {
		printf("Source file could not be read\n");
	}

	for(int i = 0; i < 3; i++) {
		printf("%s", source_array[i]);
	}

	if(InitializeArraysFromFiles(argv[2], input_array) != 0) {
		printf("Input file could not be read\n");
	}
	
	for(int i = 0; i < 3; i++) {
		printf("%s", input_array[i]);
	}
	//free(source_array);
	//free(input_array);
	return 0;
}

int InitializeArraysFromFiles(char *file_name, char *array[]) {
	FILE *f;
	char *c;
	int i = 0;
	size_t len = 0;
	ssize_t read;
	
	f = fopen(file_name, "rt");
	if (f == NULL)
		return -1;
	while((read = getline (&c, &len, f)) != -1) {
		//printf("%s", c);  //for debugging
		char *temp = malloc(sizeof(strlen(c)));
		strcpy(temp, c);
		//printf("%s", temp);  //for debugging
		array[i] = temp;
		//printf("%s", array[i]); //for debugging
		i++;
	}
	
	fclose(f);
	return 0;
}



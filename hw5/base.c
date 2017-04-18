#include <stdio.h>
#include <stdlib.h>

#include "base.h"

char *source_array;
char *input_array;

int main(int argc, char *argv[])
{
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

	//for(int i = 0; i < 3; i++) {
	//	printf("%d\n", *(source_array + i));
	//}

	if(InitializeArraysFromFiles(argv[2], input_array) != 0) {
		printf("Input file could not be read\n");
	}

	return 0;
}

int InitializeArraysFromFiles(char *file_name, char *array) {
	FILE *f;
	char c;
	int i = 0;
	
	f = fopen(file_name, "rt");
	if(f) {
		while((c = fgetc(f)) != EOF) {
			printf("%c", c);
      strcpy(array[i], c);
			//*(array + i) = c;
			i++;
		}

		fclose(f);
		return 0;
	}
	else {
		return -1;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "base.h"

#define CHUNK_SIZE 3
#define TERMS 3
#define NUM_THREADS ARRAY_SIZE*(ARRAY_SIZE/CHUNK_SIZE)

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

	f = fopen(argv[1], "rt");
	if(f == NULL)
		return -1;

	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(source_array[i], t);
		i++;
	}

	fclose(f);

	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}

	// Make the source list null terminated so that you don't need to find the size
	source_array[i][0] = NULL;

	PrintResults();

	return 0;
}

int SearchForTerm(int threadID, int chunkID, int source_index, int dif) {
	int i, j = 0;
	int start, end;
	char term[STRING_SIZE];

	#pragma omp private(threadID, chunkID, source_index, term, dif, i, j, start, end)
	{
		start = chunkID * CHUNK_SIZE;
		if(dif > 0){
			end = dif + start;
		}else{
			end = CHUNK_SIZE + start;
		}
		strcpy(term, source_array[source_index]);
	//threadID = chunkID * sizeof(source_array)/STRING_SIZE + source_index

		for(i = start; (input_array[i][0]) && (i < end); i++) {

			if(strstr(input_array[i], term)) {
				
				#pragma omp critical
				{
					if((output_array[source_index][0] != '\0') || (j != 0)) {
						sprintf(output_array[source_index], "%s, %i", output_array[source_index], i + 1);
					}else {	
						sprintf(output_array[source_index], "%i", i + 1);
					}
				}
				j++;
			}
		}
	}
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0;
	int chunk_ID = 0;
	int count = 0;
	int terms = sizeof(source_array) / STRING_SIZE;
	
	omp_set_num_threads(NUM_THREADS);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(input_array[i], t);

		if((count + 1) == CHUNK_SIZE) {
			chunk_ID++;
			count = 0;

			#pragma omp parallel
			{
				int j = omp_get_thread_num()-(chunk_ID * terms);
				SearchForTerm(omp_get_thread_num(), chunk_ID, j, 0);
			}
		}
		else {
			count++;
		}

		i++;
	}

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		chunk_ID++;
		#pragma omp parallel
		{
			int k = omp_get_thread_num()-(chunk_ID * terms);
			SearchForTerm(omp_get_thread_num(), chunk_ID, k, dif);
		}

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

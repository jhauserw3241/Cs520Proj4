#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "base.h"

#define CHUNK_SIZE 3
#define TERMS 3
#define NUM_THREADS 833333333

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

int SearchForTerm( int chunkID, int source_index, int dif) {
	int i, d, j, cID, si = 0;
	int threadID, start, end;
	char term[STRING_SIZE];
	
	omp_set_num_threads(NUM_THREADS);
	printf("number of threads to work with: %d \n", NUM_THREADS);
	printf("Life is terrible, end my suffering \n");
	#pragma omp private(threadID, chunkID, cID, source_index, si, term, dif, d, i, j, start, end)
	{
		#pragma omp parallel for
		for(cID = 0; cID <= chunkID; cID++){
			printf("suffering hs suddenly gotten less painful, but still ow \n");
			threadID = omp_get_thread_num();
			printf("threadID: %d \n", threadID);
			if(cID == chunkID)
				d = dif;

			start = cID * CHUNK_SIZE;
			if(d > 0){
				end = dif + start;
			}else{
				end = CHUNK_SIZE + start;
			}
			#pragma omp parallel for
			for(si = 0; si < source_index; si++){
				strcpy(term, source_array[si]);
				for(i = start; i < end; i++) {
		
					if(strstr(input_array[i], term)) {
					
						{
							if((output_array[si][0] != '\0') || (j != 0)) {
								sprintf(output_array[si], "%s, %i", output_array[si], i + 1);
							}else {	
								sprintf(output_array[si], "%i", i + 1);
							}	
						}
						j++;
					}
				}
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
		}
		else {
			count++;
		}
		i++;
	}

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		chunk_ID++;
	}
	SearchForTerm(chunk_ID, terms, dif);

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

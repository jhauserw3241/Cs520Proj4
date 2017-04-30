#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "base.h"

#define CHUNK_SIZE 3
#define NUM_THREADS 1
#define ARRAY_SIZE 100
#define STRING_SIZE 500

int SourceArraySize = ARRAY_SIZE;
int InputArraySize = ARRAY_SIZE;

char **source_array;
int source_count;
char **input_array;
int input_count;
char **output_array;

pthread_mutex_t search_mutex;

int main(int argc, char *argv[])
{	
	printf("Start main\n");

	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}

	// Read in source data
	if(ReadSourceData(argv[1]) != 0) {
		printf("Source file could not be read\n");
	}

	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}

	printf("Before print out results");

	PrintResults();
	
	pthread_mutex_destroy(&search_mutex);
	printf("Program completed. Exiting.\n");
	pthread_exit(NULL);

	return 0;
}

int ReadSourceData(char *filename) {
	FILE *f;
	int i = 0;

	source_array = malloc(SourceArraySize);
	
	f = fopen(filename, "rt");
	if(f == NULL)
		return -1;

	char *tempTerm = malloc(STRING_SIZE);
	while(fgets(tempTerm, STRING_SIZE, f) != NULL) {
		if(i >= SourceArraySize) {
			char **temp = (char **)calloc(SourceArraySize * 2, sizeof(char *));
			memcpy(temp, source_array, sizeof(char *) * SourceArraySize);
			free(source_array);
			source_array = temp;
			SourceArraySize *= 2;
		}

		char *term = malloc(STRING_SIZE);
		memcpy(term, tempTerm, strlen(tempTerm));
		term[strlen(term) - 1] = 0;
		source_array[i] = term;
		i++;
		source_count++;
	}

	fclose(f);
}

void *SearchForTerm(void *args) {
	printf("Hello\n");
	int j = 0;
	arg_t argt = *((arg_t *)args);
	char *term = (char *)argt.term;
	printf("Term: %s\n", term);
	int start = (int)argt.start;
	printf("Start: %d\n", start);
	int end = (int)argt.end;
	printf("End: %d\n", end);
	int diff = (int)argt.diff;
	int source_index = (int)argt.source_index;

	printf("Search for: %s\n", term);

	/*start = cID * CHUNK_SIZE;
	printf("Start of chunk: %d\n", start);

	if(diff <= 0){
		end = CHUNK_SIZE + start;
	}
	else{
		end = diff + start;
	}
	printf("End of chunk: %d\n", end);*/

	int i;
	for(i = start; (input_array[i][0]) && (i < end); i++) {
		printf("Compare: %s and %s\n", term, input_array[i]);
		if(strstr(input_array[i], term)) {
			printf("Match found!\n");
			pthread_mutex_lock(&search_mutex);
			
			char *temp = malloc(STRING_SIZE);

			if(output_array[source_index]) {
				printf("Add additional element\n");
				sprintf(temp, "%s, %i", output_array[source_index], i + 1);
			}
			else {
				printf("Start list of elements\n");
				sprintf(temp, "%i", i + 1);
			}
			output_array[source_index] = temp;
			printf("New list: %s\n", output_array[source_index]);

			pthread_mutex_unlock(&search_mutex);
			j++;
		}
	}
	pthread_exit(NULL);
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	int rc;
	int i = 0;
	int count = 0;
	pthread_t threads[NUM_THREADS];
	void *status;
	arg_t args;
	int *arg_mallocs = malloc(sizeof(arg_t));
	int thread_count = 0;

	// Initialize output array
	output_array = (char **)calloc(SourceArraySize, sizeof(char *));

	// Initialize input array
	input_array = malloc(InputArraySize);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	char *tempLine = malloc(STRING_SIZE);
	while(fgets(tempLine, STRING_SIZE, f) != NULL) {
		if(i >= InputArraySize) {
			char **temp = (char **)calloc(InputArraySize * 2, sizeof(char *));
			memcpy(temp, input_array, sizeof(char *) * InputArraySize);
			free(input_array);
			input_array = temp;
			InputArraySize *= 2;
		}

		char *line = malloc(STRING_SIZE);
		memcpy(line, tempLine, strlen(tempLine));
		line[strlen(line) - 1] = 0;
		printf("Adding line: %s\n", line);
		input_array[i] = line;

		if((count + 1) == CHUNK_SIZE) {
			int j;
			for(j = 0; j < source_count; j++) {
				printf("Looking at: %s\n", source_array[j]);
				//args = malloc(sizeof(arg_t));
				args.start = (i + 1) - CHUNK_SIZE;
				args.end = i + 1;
				args.term = source_array[j];
				args.source_index = j;
				int * arg_mal = malloc(sizeof(arg_t));
				rc = pthread_create(&threads[thread_count], NULL, SearchForTerm, &args);
				if(rc){
					printf("ERROR; return code from pthread_create() is %d\n",rc);
					exit(-1);
				}
				thread_count++;
				//free(arg_mal);
			}
			count = 0;
		}
		else {
			count++;
		}

		input_count++;
		i++;
	}
	
	printf("Done with input reading in\n");

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		int j;
		for(j = 0; j < source_count; j++) {
			args.start = j;
			args.end = i - dif;
			args.term = source_array[j];
			rc = pthread_create(&threads[thread_count], NULL, SearchForTerm, &args);
			if(rc){
				printf("ERROR; return code from pthread_create() is %d\n",rc);
				exit(-1);
			}
			thread_count++;
		}
	}

	int x;
	for(x = 0; x < NUM_THREADS; x++){
		rc = pthread_join(threads[x], &status);
		if(rc){
			printf("ERROR; return code from pthread_join() is %d\n",rc);
			exit(-1);

		}
	}

	pthread_exit(NULL);

	fclose(f);

	return 0;
}

void PrintResults() {
	int i;
	for(i = 0; source_array[i][0]; i++) {
		if(output_array[i]) {
			printf("%s %s\n", source_array[i], output_array[i]);
		}
	}
}

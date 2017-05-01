#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

#define CHUNK_SIZE 200
#define NUM_THREADS 3000
#define ARRAY_SIZE 100
#define STRING_SIZE 500

int SourceArraySize = ARRAY_SIZE;
int InputArraySize = ARRAY_SIZE;

char **source_array;
int source_count;
char **input_array;
int input_count;
char **output_array;
out_info *output_array_info;

pthread_mutex_t search_mutex;

int main(int argc, char *argv[])
{
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

	PrintResults();
	
	pthread_mutex_destroy(&search_mutex);
	pthread_exit(NULL);

	return 0;
}

int ReadSourceData(char *filename) {
	FILE *f;
	int i = 0;

	source_array = malloc(sizeof(char *) * SourceArraySize);
	
	f = fopen(filename, "rt");
	if(f == NULL)
		return -1;

	char *tempTerm = malloc(STRING_SIZE);
	while(fgets(tempTerm, STRING_SIZE, f) != NULL) {
		if(i >= SourceArraySize) {
			char **temp = (char **)calloc(SourceArraySize *2, sizeof(char *));
			memcpy(temp, source_array, sizeof(char *) * SourceArraySize);
			free(source_array);
			source_array = temp;
			SourceArraySize *= 2;
		}

		char *term = malloc(STRING_SIZE);
		strcpy(term, tempTerm);
		term[strlen(term) - 1] = 0;
		printf("Add source term: %s\n", term);
		source_array[i] = term;
		i++;
		source_count++;
	}

	fclose(f);
}

void *SearchForTerm(void *args) {
	int j = 0;
	arg_t *argst = (arg_t *)args;
	char *term = argst->term;
	printf("Term: %s\n", term);
	int start = argst->start;
	printf("Start: %d\n", start);
	int end = argst->end;
	printf("End: %d\n", end);
	int source_index = argst->source_index;

	int i;
	for(i = start; (i < input_count) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			pthread_mutex_lock(&search_mutex);

			char *out_string = output_array[source_index];
			out_info info = output_array_info[source_index];

			char *temp;
			char *num = malloc(STRING_SIZE);

			if(out_string) {
				// Check if the output string needs to be enlarged
				sprintf(num, "%d", i + 1);
				if((info.count + 2 + strlen(num)) > info.size) {
					temp = malloc(info.size * 2);
					info.size *= 2;
				}
				else {
					temp = malloc(info.size);
				}

				// Update output string
				sprintf(temp, "%s, %s", out_string, num);
				info.count += (2 + strlen(num));
			}
			else {
				// Create first output info
				info.count = 1;
				info.size = STRING_SIZE;

				// Start output string
				temp = malloc(info.size);
				sprintf(temp, "%i", i + 1);
			}

			free(output_array[source_index]);

			// Update output string
			output_array[source_index] = temp;
			output_array_info[source_index] = info;

			pthread_mutex_unlock(&search_mutex);
			j++;
		}
	}

	pthread_exit(NULL);

	return NULL;
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	int rc;
	int i = 0;
	int count = 0;
	pthread_t threads[NUM_THREADS];
	int thread_count = 0;
	void *status;
	arg_t *args;

	// Initialize output array
	output_array = (char **)calloc(source_count, sizeof(char *));
	output_array_info = (out_info *)calloc(source_count, sizeof(out_info));

	// Initialize input array
	input_array = malloc(sizeof(char *) * InputArraySize);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	char *tempLine = malloc(STRING_SIZE);
	while(fgets(tempLine, STRING_SIZE, f) != NULL) {
		//printf("i: %d\n", i);
		if((i + 1) >= InputArraySize) {
			//printf("Resize in progress\n");
			//fflush(stdout);
			char **temp = (char **)calloc(InputArraySize * 2, sizeof(char *));
			//printf("After allocate space for temp\n");
			//fflush(stdout);
			memcpy(temp, input_array, sizeof(char *) * InputArraySize);
			//printf("Before free input array\n");
			//fflush(stdout);
			free(input_array);
			//printf("After free input array\n");
			//fflush(stdout);
			input_array = temp;
			InputArraySize *= 2;
		}

		char *line = malloc(STRING_SIZE);
		strcpy(line, tempLine);
		line[strlen(line) - 1] = 0;
		//printf("Read input line: %s\n", line);
		//printf("Input count: %d\n", input_count + 1);
		input_array[i] = line;
		input_count++;

		if((count + 1) == CHUNK_SIZE) {
			int j;
			for(j = 0; j < source_count; j++) {
				args = malloc(sizeof(arg_t *));
				args->start = (i + 1) - CHUNK_SIZE;
				args->end = i + 1;
				args->term = source_array[j];
				args->source_index = j;
				rc = pthread_create(&(threads[thread_count]), NULL, SearchForTerm, args);
				thread_count++;
				if(rc){
					printf("ERROR: return code from pthread_create() is %d\n",rc);
					exit(-1);
				}
				//SearchForTerm(args);
			}
			count = 0;
		}
		else {
			count++;
		}

		i++;
	}

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		int j;
		for(j = 0; j < source_count; j++) {
			args = malloc(sizeof(arg_t *));
			args->start = i - dif;
			args->end = i;
			args->term = source_array[j];
			args->source_index = j;
			rc = pthread_create(&(threads[thread_count]), NULL, SearchForTerm, args);
			thread_count++;
			if(rc){
				printf("ERROR: return code from pthread_create() is %d\n",rc);
				exit(-1);
			}
			//SearchForTerm(args);
		}
	}

	fclose(f);

	printf("Before join all threads\n");
	fflush(stdout);

	int x;
	for(x = 0; x < thread_count; x++) {
		//printf("About to join thread\n");
		//fflush(stdout);
		rc = pthread_join(threads[x], NULL);
		//printf("After join thread\n");
		//fflush(stdout);
		if(rc) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

	printf("After join all threads\n");
	fflush(stdout);

	return 0;
}

void PrintResults() {
	int i;
	for(i = 0; i < source_count; i++) {
		if(output_array[i]) {
			printf("%s %s\n", source_array[i], output_array[i]);
		}
	}
}

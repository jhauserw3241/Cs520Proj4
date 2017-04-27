#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "base.h"

#define CHUNK_SIZE 3
#define TERMS 3
#define NUM_THREADS ARRAY_SIZE*(ARRAY_SIZE/CHUNK_SIZE)

pthread_mutex_t search_mutex;

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
	
	pthread_mutex_destroy(&search_mutex);
	printf("Program completed. Exiting.\n");
	pthread_exit(NULL);

	return 0;
}

void *SearchForTerm(void *args) {
	int j = 0;
	arg_t argt = *((arg_t *)args);
	char *term = argt.term;
	int cID = argt.chunk_ID;
	int diff = argt.diff;
	int source_index = argt.source_index;
	int start, end;
	
	start = cID * CHUNK_SIZE;

	if(diff <= 0){
		end = CHUNK_SIZE + start;
	}
	else{
		end = diff + start;
	}

	for(int i = start; (input_array[i][0]) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			pthread_mutex_lock(&search_mutex);
			if((output_array[source_index][0] != '\0') || (j != 0)) {
				sprintf(output_array[source_index], "%s, %i", output_array[source_index], i + 1);
			}
			else {
				sprintf(output_array[source_index], "%i", i + 1);
			}
			pthread_mutex_unlock(&search_mutex);
			j++;
		}
	}
	pthread_exit(NULL);
}

int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int rc;
	int i = 0;
	int count = 0;
	int cID = 0;
	int terms = sizeof(source_array) / STRING_SIZE;
	pthread_t input_threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	arg_t args;
	int *arg_mallocs = malloc(sizeof(arg_t));

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	while(fgets(temp, STRING_SIZE, f) != NULL) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(input_array[i], t);

		if((count + 1) == CHUNK_SIZE) {
			for(int j = 0; j < terms; j++) {
				args.chunk_ID = cID;
				args.term = terms[j];
				args.source_index = j;
				int * arg_mal = malloc(sizeof(arg_t));
				rc = pthread_create(&input_threads[cID+j],&attr, SearchForTerm,(void *)args);
				if(rc){
					printf("ERROR; return code from pthread_create() is %d\n",rc);
					exit(-1);
				}
				free(arg_mal);
			}
			cID++;
			count = 0;
		}
		else {
			count++;
		}

		i++;
	}

	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		cID++;
		for(int j = 0; j < terms; j++) {
			args.diff = dif;
			args.chunk_ID = cID;
			args.term = *terms[j];
			rc = pthread_create(&input_threads[cID+1+j],&attr, SearchForTerm, (void *) args);
			if(rc){
				printf("ERROR; return code from pthread_create() is %d\n",rc);
				exit(-1);
			}
		}
	}

	pthread_attr_destroy(&attr);
	for(int x = 0; x < NUM_THREADS; x++){
		rc = pthread_join(input_threads[x], &status);
		if(rc){
			printf("ERROR; return code from pthread_join() is %d\n",rc);
			exit(-1);

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

#define CHUNK_SIZE 3
#define NUM_THREADS 50
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

	source_array = malloc(SourceArraySize);
	
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
		memcpy(term, tempTerm, strlen(tempTerm));
		term[strlen(term) - 1] = 0;
		source_array[i] = term;
		i++;
		source_count++;
	}

	fclose(f);
}

void *SearchForTerm(void *args) {
	int j = 0;
	arg_t argt = *((arg_t *)args);
	char *term = argt.term;
	int start = argt.start;
	int end = argt.end;
	int diff = argt.diff;
	int source_index = argt.source_index;

	int i;
	for(i = start; (i < input_count) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			pthread_mutex_lock(&search_mutex);
			
			char *temp = malloc(STRING_SIZE);

			if(output_array[source_index]) {
				sprintf(temp, "%s, %i", output_array[source_index], i + 1);
			}
			else {
				sprintf(temp, "%i", i + 1);
			}
			output_array[source_index] = temp;

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
	node_t *thread_head;
	void *status;
	arg_t *args;

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
				pthread_t cur_thread = malloc(sizeof(pthread_t));
				rc = pthread_create(cur_thread, NULL, SearchForTerm, args);
				PushThread(&thread_head, cur_thread);
				if(rc){
					printf("ERROR: return code from pthread_create() is %d\n",rc);
					exit(-1);
				}
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
			pthread_t cur_thread = malloc(sizeof(pthread_t));
			rc = pthread_create(cur_thread, NULL, SearchForTerm, args);
			PushThread(&thread_head, cur_thread);
			if(rc){
				printf("ERROR: return code from pthread_create() is %d\n",rc);
				exit(-1);
			}
		}
	}

	fclose(f);

	while(thread_head != NULL) {
		rc = pthread_join(PopThread(&thread_head), NULL);
		if(rc) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}

		thread_head = thread_head->next;
	}

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

// Add thread to linked list
void PushThread(node_t ** head, pthread_t thread) {
	node_t *new_node = malloc(sizeof(node_t));

	new_node->thread = thread;
	new_node->next = *head;
	*head = new_node;
}

// Get first thread from linked list, remove it from list,
// and go to next element
pthread_t PopThread(node_t **head) {
	pthread_t retval;
	node_t *next_node = NULL;

	if(*head == NULL) {
		return retval;
	}

	next_node = (*head)->next;
	retval = (*head)->thread;
	free(*head);
	*head = next_node;

	return retval;
}

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
	//pthread_exit(NULL);

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
			strcpy(temp, source_array);
			free(source_array);
			source_array = temp;
			SourceArraySize *= 2;
		}

		char *term = malloc(STRING_SIZE);
		strcpy(term, tempTerm);
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
	printf("Term: %s\n", term);
	fflush(stdout);
	int start = argt.start;
	printf("Start: %d\n", start);
	fflush(stdout);
	int end = argt.end;
	printf("End: %d\n", end);
	fflush(stdout);
	int source_index = argt.source_index;

	int i;
	for(i = start; (i < input_count) && (i < end); i++) {
		if(strstr(input_array[i], term)) {
			//pthread_mutex_lock(&search_mutex);

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
				info.count += 2 + strlen(num);
			}
			else {
				// Create first output info
				info.count = 1;
				info.size = STRING_SIZE;

				// Start output string
				temp = malloc(info.size);
				sprintf(temp, "%i", i + 1);
			}

			//free(output_array[source_index]);

			// Update output string
			output_array[source_index] = temp;
			output_array_info[source_index] = info;

			//pthread_mutex_unlock(&search_mutex);
			j++;
		}
	}

	//pthread_exit(NULL);

	return NULL;
}

int ReadInputDataIntoArray(char file[]) {
	printf("Start reading in input\n");
	FILE *f;
	int rc;
	int i = 0;
	int count = 0;
	node_t *thread_head = NULL;
	void *status;
	arg_t *args;

	printf("After setup head node\n");

	// Initialize output array
	output_array = (char **)calloc(SourceArraySize, sizeof(char *));
	output_array_info = (out_info *)calloc(SourceArraySize, sizeof(out_info));

	// Initialize input array
	input_array = malloc(InputArraySize);

	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	char *tempLine = malloc(STRING_SIZE);
	while(fgets(tempLine, STRING_SIZE, f) != NULL) {
		if(i >= InputArraySize) {
			char **temp = (char **)calloc(InputArraySize * 2, sizeof(char *));
			strcpy(temp, input_array);
			free(input_array);
			input_array = temp;
			InputArraySize *= 2;
		}

		char *line = malloc(STRING_SIZE);
		strcpy(line, tempLine);
		line[strlen(line) - 1] = 0;
		printf("Add line: %s\n", line);
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
				/*pthread_t *cur_thread = (pthread_t *)malloc(sizeof(pthread_t));
				printf("After allocate thread\n");
				rc = pthread_create(cur_thread, NULL, SearchForTerm, args);
				printf("After create thread\n");
				PushThread(&thread_head, cur_thread);
				printf("After push thread\n");
				if(rc){
					printf("ERROR: return code from pthread_create() is %d\n",rc);
					exit(-1);
				}*/
				SearchForTerm(args);
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
			/*pthread_t *cur_thread = (pthread_t *)malloc(sizeof(pthread_t));
			rc = pthread_create(cur_thread, NULL, SearchForTerm, args);
			PushThread(&thread_head, cur_thread);
			if(rc){
				printf("ERROR: return code from pthread_create() is %d\n",rc);
				exit(-1);
			}*/
			SearchForTerm(args);
		}
	}

	fclose(f);

	printf("Before joining threads\n");
	fflush(stdout);

	/*while(thread_head != NULL) {
		printf("Joining a thread\n");
		fflush(stdout);
		rc = pthread_join(*PopThread(&thread_head), NULL);
		printf("After thread has been joined\n");
		fflush(stdout);
		if(rc) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}*/

	printf("Finish joining all threads\n");
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

// Add thread to linked list
void PushThread(node_t **head, pthread_t *thread) {
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	printf("Create new node\n");

	new_node->thread = thread;
	printf("Add thread to new node\n");
	fflush(stdout);
	new_node->next = *head;
	printf("Set new node to be in list\n");
	fflush(stdout);
	*head = new_node;
	printf("Set new node to head node\n");
}

// Get first thread from linked list, remove it from list,
// and go to next element
pthread_t *PopThread(node_t **head) {
	pthread_t retval;
	node_t *next_node = NULL;

	if(*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;
	retval = (*head)->thread;
	printf("Right before free\n");
	fflush(stdout);
	printf("%d\n", *head);
	free(*head);
	printf("Right after free\n");
	fflush(stdout);
	*head = next_node;

	return retval;
}

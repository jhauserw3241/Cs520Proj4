#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arg{
	char *term;
	int diff;
	int start;
	int end;
	int source_index;
}arg_t;

typedef struct thread_node {
	pthread_t *thread;
	struct thread_node *next;
} node_t;

typedef struct output_info {
	int count;
	int size;
} out_info;

int main(char *source_file, char *input_file);
int ReadInputDataIntoArray(char file[]);
int ReadSourceData(char *filename);
void *SearchForTerm(void *args);
void PrintResults();
void PushThread(node_t **head, pthread_t *thread);
pthread_t *PopThread(node_t **head);


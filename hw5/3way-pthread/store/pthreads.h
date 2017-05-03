#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arg{
	int start;
	int end;
}arg_t;

typedef struct node {
	pthread_t *thread;
	struct node *next;
} node_t;

typedef struct output_info {
	int count;
	int size;
} out_info;

int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int ReadSourceData(char *filename);
void *SearchForTerm(void *args);
void PrintResults();
void PushThread(node_t **head, pthread_t *thread);
pthread_t *PopThread(node_t **head);

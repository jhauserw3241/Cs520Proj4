#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 50000
#define STRING_SIZE 300
#define IN_SIZE 100000
#define IN_STRING 2500
#define OUT_SIZE 10000

char sub_out[ARRAY_SIZE][30000]; 
char output_array[1][1]; 
char source_array[ARRAY_SIZE][50]; //60MB
char **input_array; //500MB

int src_count = 0, in_count = 0;
	
int main(int argc, char *argv[]);
int ReadInputDataIntoArray(char file[]);
int SearchForTerm(char term[], int source_index, int start, int end);
void PrintResults();
void init_array(char *arr[], int count);
//void append(struct list_node** head_ref, char new_data[]);

struct list_node {
	char data[16];
	struct list_node *next;
};
struct list_node *sub_list[1];
struct list_node *out_list[1];
int SearchForTermLinkedList(char term[], int source_index, int start, int end);
void PrintResultsLinkedList();
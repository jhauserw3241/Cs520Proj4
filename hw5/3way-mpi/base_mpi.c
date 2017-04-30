#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "base_mpi.h"

#define CHUNK_SIZE 3



int main(int argc, char *argv[])
{	
	
	int i , j;
	
	// Verify that the correct number of arguments were provided
	if(argc != 3)
	{
		printf("The structure of the command should be:\n");
		printf("    ./base source_file.txt input_file.txt\n");
		return -1;
	}
	
	//Init MPI
	MPI_Init(NULL, NULL);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	
	
	/*sub_out = (char**) malloc (50000 * sizeof(char*));
	for ( i = 0; i < 50000; i++ )
	{
		sub_out[i] = (char*) malloc(2000000 * sizeof(char));
	}*/
	input_array = (char**) calloc (IN_SIZE, sizeof(char*));
	for ( i = 0; i < IN_SIZE; i++ )
	{
		input_array[i] = (char*) calloc(IN_STRING, sizeof(char));
	}
	//Read in source data
	if(ReadSourceDataIntoArray(argv[1]) != 0) {
		printf("Source data could not be read\n");
	}
	
	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}
	
	//print num cores used, terms and lines
	if(world_rank == 0)
	{
		printf("Number of cores used: %d\n", world_size);
		printf("Source terms read: %d\n", src_count);
		printf("Input lines read: %d\n", in_count);
	}
	
	
	int count = src_count/world_size;
	char sub_source[count][50];
	
	//scatter jobs to cores
	MPI_Scatter(source_array, count*50, MPI_BYTE, sub_source, count*50, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	//debugging printing the sub_source
	/*
	for (j = 0; j < count; j++)
	{
		printf("Node %d, j = %d, sub_source: %s\n",world_rank, j, sub_source[j]);
	}*/
	
	
	for(i = 0; i < count; i++)
	{
		SearchForTerm(sub_source[i],i, 0, in_count);
	}
	/*for(i = 0; i < count; i++)
	{
		SearchForTermLinkedList(sub_source[i], i, 0, in_count);
	}*/
	/*for (j = 0; j < count; j++)
	{
		printf("%s\n",sub_out[j]);
	}*/
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	//printing results
	for (j = 0; j < count; j++)
	{
		if (strlen(sub_out[j]) != 0)
		{
			int index;
			index = world_rank*count + j;
			printf("%s %s\n",source_array[index], sub_out[j]);
		}
	}
	
	//MPI_Gather(sub_out, count*100000, MPI_BYTE, output_array, count*100000, MPI_BYTE, 0, MPI_COMM_WORLD);
	//MPI_Gather(sub_list, count*sizeof(struct list_node), MPI_CHAR, out_list, count*sizeof(struct list_node), MPI_CHAR, 0, MPI_COMM_WORLD);
	
	/*if (world_rank == 0){
		for (j = 0; j < src_count; j++)
		{
			printf("Node %d, j = %d, output data: %s\n",world_rank, j, output_array[j]);
		}
	}*/
	
	if(world_rank == 0)
	{
		//PrintResults();
		//PrintResultsLinkedList();
	}
	
	free_array();
	MPI_Finalize();
	
	
	return 0;
}

int SearchForTerm(char term[], int source_index, int start, int end) {
	int j = 0;
	int i;

	for(i = start; i < end; i++) {
		if(strstr(input_array[i], term)) {
			if((sub_out[source_index][0] != '\0') || (j != 0)) {
				sprintf(sub_out[source_index], "%s, %i", sub_out[source_index], i + 1);
			}
			else {
				sprintf(sub_out[source_index], "%i", i + 1);
			}
			j++;
		}
	}
}

int ReadSourceDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0;
	
	f = fopen(file, "rt");
	if(f == NULL)
		return -1;

	while((fgets(temp, STRING_SIZE, f) != NULL) && src_count < 50000) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(source_array[i], t);
		i++;
		src_count++;
	}
	
	// Make the source list null terminated so that you don't need to find the size
	source_array[i][0] = NULL;

	fclose(f);
	
	return 0;
}
	
int ReadInputDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0, j = 0;
	int count = 0;
	int terms = sizeof(source_array) / STRING_SIZE;


	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	while((fgets(temp, STRING_SIZE, f) != NULL) && in_count < 100000) {
		char *t = temp;
		t[strlen(temp) - 1] = 0;
		strcpy(input_array[i], t);
		/*
		if((count + 1) == CHUNK_SIZE) {
			for(j = 0; j < terms; j++) {
				SearchForTerm(source_array[j], j, (i + 1) - CHUNK_SIZE, (i + 1));
			}
			count = 0;
		}
		else {
			count++;
		}
*/
		i++;
		in_count++;
	}
/*
	int dif = i % CHUNK_SIZE;
	if(dif != 0) {
		for(j = 0; j < terms; j++) {
			SearchForTerm(source_array[j], j, i - dif, i);
		}
	}
*/
	// Add NULL to end so we don't need to keep track of the array size
	input_array[i][0] = NULL;

	fclose(f);

	return 0;
}

void free_array(){
	int i;
	for ( i = 0; i < IN_SIZE; i++ )
	{
		free(input_array[i]);
	}
	free(input_array);
}

void PrintResults() {
	int i;
	for(i = 0; i<src_count; i++) {
		if (strlen(output_array[i]) != 0)
			printf("%s %s\n", source_array[i], output_array[i]);
	}
}

/*
int SearchForTermLinkedList(char term[], int source_index, int start, int end) {
	int j = 0;
	int i;
	char temp[16];

	for(i = start; i < end; i++) {
		if(strstr(input_array[i], term)) {
			if( j > 0)
				sprintf(temp, ", %i", i + 1);
			else
				sprintf(temp, "%i", i + 1);
			append(&sub_list[source_index], temp);
			j++;
		}
	}
	//debugging sub_out in search
	//printf("Sub out in Search: %s\n", sub_out[source_index]);
}

void PrintResultsLinkedList() {
	int i;
	for(i = 0; source_array[i][0]; i++) {
		//if (strlen(output_array[i]) != 0)
		//	printf("%s %s\n", source_array[i], output_array[i]);
		if (out_list[i] != NULL)
		{
			printf("\n%s ", source_array[i]);
		}
		while (out_list[i] != NULL)
		{
			printf("%s", out_list[i]->data);
			out_list[i] = out_list[i]->next;
		}
	}
	printf("\n");
}

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new node at the end  */
/*void append(struct list_node** head_ref, char new_data[])
{
    // 1. allocate node 
    struct list_node *new_node = (struct list_node*) malloc(sizeof(struct list_node));
 
    struct list_node *last = *head_ref;  // used in step 5
	
    // 2. put in the data  
    //new_node->data = new_data;
	strcpy(new_node->data, new_data);
 
    // 3. This new node is going to be the last node, so make next 
    //    of it as NULL
    new_node->next = NULL;
 
    // 4. If the Linked List is empty, then make the new node as head 
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
      
    // 5. Else traverse till the last node 
    while (last->next != NULL)
        last = last->next;
  
    // 6. Change the next of last node 
    last->next = new_node;
    return;    
}*/

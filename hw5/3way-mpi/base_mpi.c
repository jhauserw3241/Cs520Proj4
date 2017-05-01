#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "base_mpi_old.h"

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
	
	//Read in source data
	if(ReadSourceDataIntoArray(argv[1]) != 0) {
		printf("Source data could not be read\n");
	}
	
	//debug how many process is reading data
	//printf("First read \n");
	
	
	// Read in input data
	if(ReadInputDataIntoArray(argv[2]) != 0) {
		printf("Input file could not be read\n");
	}
	
	//debug how many process is reading data
	//printf("Second read \n");
	
	
	//Init MPI
	MPI_Init(NULL, NULL);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	//print num cores used
	if(world_rank == 0) {
		printf("Number of cores used: %d\n", world_size);
		printf("Number of words read: %d\n", src_count);
		printf("Number of lines read: %d\n", in_count);
	}
	
	//DEBUGGING
	
	//printf("Size of src: %i, size of src div by string size: %i\n", sizeof(source_array), sizeof(source_array)/STRING_SIZE);
	//printf("Counter: %d\n", counter);
	//printf("Input lines: %d\n", in_counter);
	/*for(i=0; i< in_counter; i++)
	{
		if(world_rank == 0)
			printf("%s\n",input_array[i]);
	}*/		
	
	
	int count = src_count/world_size;
	char sub_source[count][STRING_SIZE];
	//printf("rank: %d, size %d\n", world_rank, world_size);
	MPI_Scatter(source_array, count*STRING_SIZE, MPI_BYTE, sub_source, count*STRING_SIZE, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	//debugging printing the sub_source
	/*
	for (j = 0; j < count; j++)
	{
		printf("Node %d, j = %d, sub_source: %s\n",world_rank, j, sub_source[j]);
	}
	*/
	
	for(i = 0; i < count; i++)
	{
		SearchForTerm(sub_source[i],i, 0, in_count);
	}
	
	//debugging printing sub_out
	/*
	for (j = 0; j < count; j++)
	{
		printf("Node %d, j = %d, sub_out: %s\n",world_rank, j, sub_out[j]);
	}
	*/
	
	MPI_Gather(sub_out, count*10000, MPI_BYTE, output_array, count*10000, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	if(world_rank == 0)
		PrintResults();

	return 0;
	MPI_Finalize();
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
	//debugging sub_out in search
	//printf("Sub out in Search: %s\n", sub_out[source_index]);
}
int ReadSourceDataIntoArray(char file[]) {
	FILE *f;
	char temp[STRING_SIZE];
	int i = 0;
	int limit = 50000;
	
	f = fopen(file, "rt");
	if(f == NULL)
		return -1;

	while((fgets(temp, STRING_SIZE, f) != NULL) && (src_count < limit)) {
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
	int limit = 50000;
	//int terms = sizeof(source_array) / STRING_SIZE;


	f = fopen(file, "rt");
	if(f == NULL)
		return -1;
	
	while((fgets(temp, STRING_SIZE, f) != NULL) && (in_count < limit)) {
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

void PrintResults() {
	int i;
	for(i = 0; source_array[i][0]; i++) {
		if (strlen(output_array[i]) != 0)
			printf("%s %s\n", source_array[i], output_array[i]);
	}
}

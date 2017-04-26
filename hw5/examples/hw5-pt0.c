#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE 50000
#define STRING_SIZE 300

/*char getRandomChar()
{
	int randNum = 0;
	char randChar = ' ';

	randNum = 26 * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
	randNum = randNum + 97;				// scale to 'a'
	randChar = (char) randNum;

	// printf("%c", randChar);
	return randChar;
}

void init_arrays()
{
  int i, j, randNum; 

  for ( i = 0; i < ARRAY_SIZE; i++) {
	for ( j = 0; j < STRING_SIZE; j++ ) {
		 char_array[i][j] = getRandomChar();
	}
  }

  for ( i = 0; i < 26; i++ ) {
  	char_counts[i] = 0;
  }
}*/

int[][] count_array(char ** words, char ** input)
{
  char * word;
  int i, j, x = 0;
  int word_counts[words.length][ARRAY_SIZE];
  for(i = 0; i < words.length; i++){
     word = words[i];
     for( j = 0; j < input.length; j++){
     	if(strstr(input[j],word) != NULL){
	   word_counts[i][x] = j;
	   x++;
	}
     }
  }
}

void print_results(char ** words, char ** input)
{ 
  char * word;
  int i, j = 0;
  int word_counts[words.length][ARRAY_SIZE] = count_array(words, input);

  for ( i = 0; i < words.length; i++ ) {
     word = words[i];
     printf("Instances of %s: ", word);
     for(j = 0; j < word_counts[i].length; j++){
     	if(word_counts[i][j] != NULL){
	    printf("%d, ", word_counts[i][j];
	}
     }
  }
}

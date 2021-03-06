#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "pthreads.h"

int main() {
	int i, j=0;
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;

	gettimeofday(&t1, NULL);

	
	printf("DEBUG: starting loop on %s\n", getenv("HOST"));

	int er = run("/scratch/dan/words_4-8chars_50k", "/scratch/dan/wiki.1Mshort");

	gettimeofday(&t2, NULL);


	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %s, %s, %f", "pthreads", getenv("NSLOTS"),  elapsedTime);

}

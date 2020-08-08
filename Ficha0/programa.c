#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define MAX 30 

int main(int argc, char *argv[]){
	
	int *vector = (int *) malloc(MAX * sizeof(int));
	
	fill(vector, MAX, 0);
	printv(vector, MAX);

	return 0;
}

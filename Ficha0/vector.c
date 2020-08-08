#include <stdio.h>
#include "vector.h"

void fill (int *vector, int size, int value) {
	int i;
	for(i=0; i<size; i++)
		vector[i] = value;
		//*vector+i = value;
}

int find (int *vector, int size, int value) {
	int i, res=-1;
	for(i=0; i<size && res == -1; i++)
		if(vector[i] == value)
			res = i;
	return res;
}

void printv (int *vector, int size) {
	int i;
	printf("[");
	for(i=0; i<size; i++)
		printf("%d, ",vector[i]);
	printf("]\n");
}

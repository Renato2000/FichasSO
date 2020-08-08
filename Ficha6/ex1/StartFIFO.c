#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	int fifo;

	fifo = mkfifo("./fifo", 0666);

	if(fifo == -1){
		perror("Error creating fifo");
		return -1;
	}

	return 0;
}
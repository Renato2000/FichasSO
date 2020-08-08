#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argv, char* argc[]){
	int fd = open("File.txt", O_CREAT | O_WRONLY, 0644);
	int n = atoi(argc[1]) * 100000;
	for(int i=0; i<n; i++)
		write(fd,"..........", 10);
	close(fd);
}
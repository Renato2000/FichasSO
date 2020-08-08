#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX 1024

int main(int argc, char* argv[]){
	//char buff[MAX];
	//int bytes_read;
	int fd;

	if((fd = open("fifo", O_WRONLY)) == 0){
		perror("Open");
	}
	else{
		printf("Opened fifo for writing\n");
	}

	if(write(fd, argv[1], strlen(argv[1])) == -1){
		perror("Write");
	}

	close(fd);

	return 0;
}
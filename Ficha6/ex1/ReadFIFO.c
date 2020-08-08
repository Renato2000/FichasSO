#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define MAX 1024

int main(int argc, char* argv){
	int fd;
	int bytes_read;
	char buff[MAX];

	if((fd = open("fifo", O_RDONLY)) == -1){
		perror("Open");
		return -1;
	}
	else{
		printf("Fifo opened for reading\n");
	}

	while((bytes_read = read(fd, &buff, MAX)) > 0){
		write(1, &buff, bytes_read);
	}

	if(bytes_read == 0){
		printf("EOF\n");
	}
	else{
		perror("Read");
	}

	close(fd);
	unlink("fifo");

	return 0;
}
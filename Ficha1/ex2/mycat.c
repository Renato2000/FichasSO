#include <unistd.h>
#include <fcntl.h>

#define TAM 1000

void main(int argc, char* argv[]){
	int fd1, fd2, r;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = 1;
	char buff[TAM];
	r = read(fd1, buff, TAM);
	while(r){
		write(fd2, buff, r);
		r = read(fd1, buff, TAM);
	}

	close(fd1);
	close(fd2);
}
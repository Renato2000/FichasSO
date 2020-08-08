#include <unistd.h>
#include <fcntl.h>

#define TAM 100

void main(int argc, char* argv[]){
	int fd1, fd2;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_CREAT | O_WRONLY, 0644);
	char buff[TAM];
	int r = 1;

	r = read(fd1, buff, TAM);
	while(r){
		write(fd2, buff, r);
		r = read(fd1, buff, TAM);
	}

	close(fd1);
	close(fd2);
}
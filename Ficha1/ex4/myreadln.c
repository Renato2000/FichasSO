#include <unistd.h>
#include <fcntl.h>

#define TAM 1000

void main(int argc, char* argv[]){
	int r, fd = open(argv[1], O_RDONLY), i;
	char buff[TAM];
	r = read(fd, buff, TAM);
	for(i=0; i<r && buff[i] != '\n'; i++);
	write(1, buff, i+1);
}
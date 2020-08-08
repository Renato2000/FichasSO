#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define TAM 100

int readln(int fd, char* buff, int tam){
	int r, i;
	r = read(fd, buff, tam);
	for(i=0; i<r && buff[i] != '\n'; i++);
	if(buff[i] == '\n') i++;
	return i;
}

void main(int argc, char* argv[]){
	char buff[TAM];
	char aux[10];
	int n, i=0;
	while(n = readln(0, buff, TAM-1)){
		if(buff[0] != '\n'){
			sprintf(aux, "\t%d:   ", i++);
			write(1, aux, 10);
			write(1, buff, n);
		}
	}
}

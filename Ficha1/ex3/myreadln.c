#include <unistd.h>
#include <fcntl.h>

//Versão caracter a caracter
void main(int argc, char* argv[]){
	int r, fd = open(argv[1], O_RDONLY);
	char buff[1];
	r = read(fd, buff, sizeof(char));
	while(r>0 && buff[0] != '\n'){
		write(1, buff, 1);
		r = read(fd, buff, sizeof(char));
	}
	if(buff[0] == '\n') write(1, buff, 1);
}
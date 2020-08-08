#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int res=0;
	int i=0;
	int nr_lines = 12;
	char buffer;
	char line[1024];

	int ifd = open("/etc/passwd", O_RDONLY);
	int ofd = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	int efd = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

	res = dup2(ifd, 0);
	res = dup2(ofd, 1);
	res = dup2(efd, 2);


	close(ifd);
	close(ofd);
	close(efd);

	while(nr_lines>0){
		while(read(0, &buffer, 1) != 0){
			line[i] = buffer;
			i++;
			if(buffer == '\n') break;
		}
		
		write(1, line, i);
		write(2, line, i);

		i=0;
		nr_lines--;
	}
	return 0;
}
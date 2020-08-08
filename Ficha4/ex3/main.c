#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int res=0;
	int fork_ret, wait_ret, status;
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

	fork_ret = fork();

	if(fork_ret == 0){
		printf("Sou o filho %d\n", getpid());

		execlp("wc", "wc", NULL);

		perror("reached return");

		_exit(0);
	}
	else{
		printf("Sou o pai %d\n", getpid());

		wait_ret = wait(&status);
	}

	return 0;
}
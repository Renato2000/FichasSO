#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
	int p[2];
	int ret;

	pipe(p);

	switch(fork()){
		case -1:
			perror("fork");
			return -1;
		case 0: //Executa o codigo do filho
			close(p[0]);
			dup2(p[1], 1); //redireciona o stdout
			close(p[1]);
			ret = execlp("ls", "ls", "/etc", NULL);
			_exit(ret);
		default:
			close(p[1]);
			dup2(p[0], 0); //redireciona o stdint
			close(p[0]);
			//wait(NULL);
			ret = execlp("wc", "wc", "-l", NULL);
			_exit(ret);
	}

	return -1;
}
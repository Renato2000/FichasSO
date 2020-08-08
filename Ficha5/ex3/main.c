#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define TAM 1024

int main(int argc, char* argv[]){
	int p[2];
	int n, status;
	char buff[TAM];

	if(pipe(p) == -1){
		perror("pipe");
		return -1;
	}

	switch(fork()){
		case -1: //Erro
			perror("Fork");
			break;
		case 0: //Filho
			close(p[1]); //Fecha o descritor de escrita
			dup2(p[0], 0); //Redireciona o stdin para o pipe de leitura
			close(p[0]); //Fecha o pipe de leitura
			int res = execlp("wc", "wc", NULL);
			_exit(res);
			break;
		default: //Pai
			close(p[0]); //Fecha o descritor de leitura
			while(n = read(0, buff, TAM)){
				write(p[1], buff, n);
			}
			close(p[1]);
			wait(&status);
			break;
	}
}
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int pai2filho(){
	int p[2];
	char line[] = "linha1\n";
	char buffer[20];
	int res, status;

	if(pipe(p) == -1){
		perror("pipe");
		return -1;
	}

	switch(fork()){
		case -1:
			perror("fork");
			return -1;
		case 0: //Filho
			close(p[1]); //Fecha o descritor de escrita
			res = read(p[0], &buffer, sizeof(buffer));
			printf("[FILHO]: read %s from pipe\n", buffer);
			close(p[0]);
			_exit(0);
		default: //Pai
			close(p[0]); //Fecha o descritor de leitura
			//sleep(5);
			write(p[1], &line, sizeof(line));
			printf("[PAI]: wrote line to pipe\n");
			close(p[0]);
			wait(&status);
	}	
	return 0;
}

int filho2pai(){
	int p[2];
	char line[] = "linha2\n";
	char buffer[20];
	int res, status;

	if(pipe(p) == -1){
		perror("pipe");
		return -1;
	}

	switch(fork()){
		case -1:
			perror("fork");
			return -1;
		case 0: //Filho
			close(p[0]); //Fecha o descritor de leitura
			write(p[1], &line, sizeof(line));
			printf("[FILHO]: wrote line to pipe\n");
			close(p[1]);
			_exit(0);
		default: //Pai
			close(p[1]); //Fecha o descritor de escrita
			//sleep(5);
			res = read(p[0], &buffer, sizeof(buffer));
			printf("[PAI]: read %s from oipe\n", buffer);
			close(p[0]);
			wait(&status);
	}	
	return 0;
}

int main(){
	//pai2filho();
	filho2pai();
	return 0;
}
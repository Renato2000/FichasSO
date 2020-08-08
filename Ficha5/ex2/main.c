#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int pai2filho(){
	int p[2];
	char * lines[] = {"linha1\n", "linha2\n", "linha3\n", "linha4\n", "linha5\n"};
	char buffer;
	int num_lines = 5;
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
			while(read(p[0], &buffer, 1) > 0){
				printf("[FILHO]: read %c from pipe\n", buffer);
			}
			printf("[FILHO] out of while\n");
			close(p[0]);
			_exit(0);
		default: //Pai
			close(p[0]); //Fecha o descritor de leitura
			//sleep(5);
			for(int i = 0; i < num_lines; i++){
				write(p[1], lines[i], sizeof(lines[i]));
				printf("[PAI]: wrote %s to pipe\n", lines[i]);
			}
			printf("[PAI]: out of wait\n");
			close(p[1]);
			wait(&status);
	}	
	return 0;
}

/* Não está feito
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
			close(p[0]); //Fecha o descritor de leiture
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
*/

int main(){
	pai2filho();
	//filho2pai();
	return 0;
}
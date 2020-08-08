#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM 100

int run(char* command){
	char* string;
	char* exec_args[20];
	int i=0;	

	string = strtok(command, " \n");
	while(string != NULL){
		exec_args[i] = string;
		string = strtok(NULL, " \n");
		i++;
	}
	exec_args[i] = NULL;

	int exec_ret = execvp(exec_args[0], exec_args);
	_exit (exec_ret);
}

int readln(int fd, char* buff, int tam){
	int r, i;
	r = read(fd, buff, tam);
	for(i=0; i<r && buff[i] != '\n'; i++);
	return i;
}

int main(int argc, char* argv[]){
	char* buff = malloc(sizeof(char) * TAM);
	int background_flag = 0, exit_flag = 0;
	int n;
	pid_t pid;
	do{
		write(1,">",1);
		n = readln(1, buff, TAM-1);
		if(n>1 && buff[n-2] == '&') background_flag = 1;
		if(strcmp("exit\n", buff)){
			if((pid = fork()) == 0){
				run(buff);
			}
			if(!background_flag){
				waitpid(pid,NULL,0);
			}
		}
		else{
			exit_flag = 1;
		}
	}while(!exit_flag && n>0);
	free(buff);
	return 0;
}
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM 1024
#define MAX_COMMANDS 100

int redirect(char* op, char* file){
	int fd;

	if(!strcmp(op, "<")){
		fd = open(file, O_RDONLY);
		dup2(fd, 0);
		close(fd);
	}
	else if(!strcmp(op, ">")){
		printf("aaa\n");
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
		dup2(fd, 1);
		close(fd);
	}
	else if(!strcmp(op, ">>")){
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0666);
		dup2(fd, 1);
		close(fd);
	}
	else if(!strcmp(op, "2>")){
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
		dup2(fd, 2);
		close(fd);
	}
	else if(!strcmp(op, "2>>")){
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0666);
		dup2(fd, 2);
		close(fd);
	}
	return 0;
}

int executa(char* command){
	char* string;
	char* exec_args[20];
	int i=0, res=0, status;	

	int fdin = dup(0);
	int fdout = dup(1);
	int fderror = dup(2);

	string = strtok(command, " \n");
	while(string != NULL){
		if(!strcmp(string, "<") || !strcmp(string, ">") || !strcmp(string, ">>") || !strcmp(string, "2>") || !strcmp(string, "2>>")){
			char op[4];
			strcpy(op, string);
			string = strtok(NULL, " \n");
			redirect(op, string);
			string = strtok(NULL, " \n");
		}
		else{
			exec_args[i] = string;
			string = strtok(NULL, " \n");
			i++;
		}
	}
	exec_args[i] = NULL;

	int exec_ret = execvp(exec_args[0], exec_args);
	_exit (exec_ret);
}

int readln(int fd, char* buff, int N){
	int r = read(fd, buff, N);
	int i;
	for(i=0; i<r && buff[i] != '\n'; i++);
	if(i==N) buff[N] = '\n';
	return i+1;
}

int main(int agrc, char* argv[]){
	int background = 0, exit = 0, n, nr_comandos = 0;
	char* command;
	pid_t pid;
	do{
		write(1, ">", 1);
		char* buff = malloc(sizeof(char) * TAM);
		n = readln(1, buff, TAM);

		char* commands[MAX_COMMANDS];

		if(n>1 && buff[n-2] == '&') background = 1;

		for(nr_comandos = 0; (command = strsep(&buff, "|")) != NULL; nr_comandos++){
			commands[nr_comandos] = strdup(command);
		}
		
		//for(int i=0; i<nr_comandos; i++) printf("[Debug] %s\n", commands[i]);
		
		if(strcmp("exit\n", commands[0])){
			if(nr_comandos == 1){ //Para o caso de ser executado apenas um comando
				switch(fork()){
					case -1:
						perror("fork");
						return -1;
					case 0:
						_exit(executa(commands[0]));
				}
			}
			else{ //Para o caso de ser executado mais de um comando
				int pipes[nr_comandos-1][2]; //Cria n-1 pipes usados na comunicação entre os processos

				for(int c = 0; c < nr_comandos; c++){
					if(c == 0){
						if(pipe(pipes[c]) != 0){
							perror("pipe");
							return -1;
						}
						switch(fork()){
							case -1:
								perror("fork");
								return -1;
							case 0:
								close(pipes[c][0]);
								
								dup2(pipes[c][1], 1);
								close(pipes[c][1]);

								executa(commands[c]);

								_exit(0);
							default:
								close(pipes[c][1]);
						}
					}
					else if(c == nr_comandos - 1){
						switch(fork()){
							case -1:
								perror("fork");
								return -1;
							case 0:
								dup2(pipes[c-1][0], 0);
								close(pipes[c-1][0]);

								executa(commands[c]);
								_exit(0);
							default:
								close(pipes[c-1][0]);
						}
					}
					else{
						if(pipe(pipes[c]) != 0){
							perror("pipe");
							return -1;
						}
						switch(fork()){
							case -1:
								perror("fork");
								return -1;
							case 0:
								close(pipes[c][0]);

								dup2(pipes[c][1], 1);
								close(pipes[c][1]);

								dup2(pipes[c-1][0], 0);
								close(pipes[c-1][0]);

								executa(commands[c]);
								_exit(0);
							default:
								close(pipes[c][1]);
								close(pipes[c-1][0]);
						}
					}
				}

				for(int i = 0; i < nr_comandos; i++){
					free(commands[i]);
				}
			}
			for(int i = 0; i < nr_comandos; i++){
				wait(NULL);
			}
		}
		else{
			if(nr_comandos == 1) exit = 1;
		}
		if(background == 0){
			waitpid(pid, NULL, 0);
		}
		free(buff);
	}while(!exit && n>0);
	return 0;
}
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM 1024

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

	int fork_ret = fork();
	if(fork_ret == 0){
		int exec_ret = execvp(exec_args[0], exec_args);
		_exit (exec_ret);
	}
	else{
		if(fork_ret != -1){
			int wait_ret = wait(&status);

			dup2(fdin, 0);
			dup2(fdout, 1);
			dup2(fderror, 2);
			close(fdin);
			close(fdout);
			close(fderror);

			if(WIFEXITED(status)){
				res = WEXITSTATUS(status);
			}
			else{
				res = -1;
			}
		}
		else{
			res = -1;
		}
	}

	return res;
}

int readln(int fd, char* buff, int N){
	int r = read(fd, buff, N);
	int i;
	for(i=0; i<r && buff[i] != '\n'; i++);
	if(i==N) buff[N] = '\n';
	return i+1;
}

int main(int agrc, char* argv[]){
	int background = 0, exit = 0, n;
	pid_t pid;
	do{
		write(1, ">", 1);
		char* buff = malloc(sizeof(char) * TAM);
		n = readln(1, buff, TAM);
		if(n>1 && buff[n-2] == '&') background = 1;
		if(strcmp("exit\n", buff)){
			if((pid = fork()) == 0){
				_exit(executa(buff));
			}
			if(background == 0){
				waitpid(pid, NULL, 0);
			}
		}
		else{
			exit = 1;
		}
		free(buff);
	}while(!exit && n>0);
	return 0;
}

/*
< - redireciona o input para o ficheiro
> - redireciona o output para o ficheiro 
>>- redireciona o output para o ficheiro e remove o conteudo (faz append)
2> - redireciona para o stderror
2>> - redireciona para o stderror e faz append
*/

//depois de fazer dup2, temos de voltar a redirecionar para o stdout e stdin
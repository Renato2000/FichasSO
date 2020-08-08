#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int mySystem(char* command){
	int fork_ret, exec_ret, wait_ret, status, res;
	char* exec_args[20];
	char* string;
	int i=0;

	string = strtok(command, " ");

	while(string != NULL){
		exec_args[i] = string;
		string = strtok(NULL, " ");
		i++;
	}

	exec_args[i] = NULL;

	fork_ret = fork();
	if(fork_ret == 0){//Codigo do filho
		exec_ret = execvp(exec_args[0], exec_args);
		//printf("%d\n", exec_ret);
		_exit(exec_ret);
	}
	else{//Codigo do pai
		if(fork_ret != -1){
			//para garantir que o pai espera por aquele filho e não por um filho qualquer(usando processos em background)
			wait_ret = waitpid(fork_ret,&status,0);
			if(WIFEXITED(status)){
				res = WIFEXITED(status);
			}
			else{
				res = -1;
			}
		}
	}

	return res;
}

int main(int argc, char* argv[]){
	char comando1[] = "ls -l -a -h";
	char comando2[] = "sleep 10";
	char comando3[] = "ps";
	int ret;

	printf("A executr mySystem para %s\n", comando1);
	ret = mySystem(comando1);
	printf("ret: %d\n", ret);

	printf("A executr mySystem para %s\n", comando2);
	ret = mySystem(comando2);

	printf("A executr mySystem para %s\n", comando3);
	ret = mySystem(comando3);

	return 0;
}
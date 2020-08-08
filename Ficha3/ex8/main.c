#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int executa(char* programa){
	int n=0	, exit=0, status;
	while(!exit){
		if(!fork()){
			int exec_ret = execl(programa, programa, NULL); //devolve o resultado da função se correr bem e -1 se correr mal
			_exit(exec_ret);
		}

		wait(&status);
		if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
			exit = 1;
		}
		n++;
	}
	return n;
}

int find(int* pids, int N, int pid){
	int i;
	for(i=0; i<N; i++){
		if(pids[i] == pid) return i;
	}
	return -1;
}

int main(int argc, char* argv[]){
	int pids[argc], values[argc];
	int status, terminated_pid, i;	

	if(argc < 2){
		printf("Input inválido. Deve ser ./program [programa1, ...]\n");
		return 1;
	}

	for(i=1; i<argc; i++){
		if((pids[i] = fork()) == 0){
			_exit(executa(argv[i]));
		}
	}	

	for(i=1; i<argc; i++){
		terminated_pid = wait(&status);
		if(WIFEXITED(status)){
			values[find(pids+1, argc-1, terminated_pid) + 1] = WEXITSTATUS(status);
		} 
	}

	printf("------------------------------------------------------\n");
	for(i=1; i<argc; i++){
		printf("Programa: %s Vezes executado: %d\n", argv[i], values[i]);
		printf("------------------------------------------------------\n");
	}

	return 0;
}
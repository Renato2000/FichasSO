#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

void main(int argc, char* argv[]){
	pid_t pid;
	int status;

	if((pid = fork()) == 0){//Processo filho;
		pid_t pai = getppid();
		pid_t this = getpid();
		printf("Pid do processo pai: %d\nPid deste processo: %d\n", pai, this);
		_exit(0);
	}
	else{//Processo pai
		pid_t child = wait(&status);//Se isto não for colocado, o processo pai sera executado primeiro
		pid_t pai = getppid();
		pid_t this = getpid();
		printf("Pid do processo pai: %d\nPid deste processo: %d\nPid do processo filho: %d\n", pai, this, pid);
	}
}
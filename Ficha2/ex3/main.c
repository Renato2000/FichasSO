#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

//Padrão sequencial
void main(int argc, char* argv[]){
	pid_t pid;
	int nproc = 10;
	int status;

	for(int i = 1; i <= nproc; i++){
		if((pid = fork()) == 0){
			printf("[proc #%d] pid %d\n", i, getpid());
			_exit(i);
		}else{
			pid_t terminated_pid = wait(&status);
			printf("[pai] process %d exited. exit code: %d\n", terminated_pid, WEXITSTATUS(status));
		}
	}
}
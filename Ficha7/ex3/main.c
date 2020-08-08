#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int *pids;
int nr_pids;

void timeout_handler(int sig){
	for(int i = 0; i < nr_pids; i++){
		printf("timeout - grep %d\n", pids[i]);
		if(pids[i] > 0) kill(pids[i], SIGKILL);
	}
}

int main(int argc, char* argv[]){
	nr_pids = argc-2;
	pids = malloc(sizeof(int) * nr_pids);
	int status, find = 1;

	if(signal(SIGALRM, timeout_handler) < 0){
		perror("signal SIGALRM");
		_exit(-1);
	}

	for(int i = 0; i < nr_pids; i++){
		if((pids[i] = fork()) == 0){
			int ret = execlp("grep", "grep", argv[1], argv[i+2], NULL);
			_exit(-1);
		}
	}

	alarm(10);

	for(int i = 1; i <= nr_pids && find; i++){
		int terminated_pid = wait(&status);

		if(WIFEXITED(status)){
			switch(WEXITSTATUS(status)){
				case 0:
					printf("[PID] %d encontrou a palavra.\n", terminated_pid);
					find = 0;
					for(int j = 0; j < nr_pids; j++)
						if(pids[j] != terminated_pid && pids[i] > 0){
							kill(pids[j], SIGKILL);
						} 
					break;
				case 1: 
					printf("[PID] %d não encontrou a palavra.\n", terminated_pid);
					break;
			}
		}
		else{
			printf("Processo %d interrompido.\n", terminated_pid);
		}
	}
	return 0;
}
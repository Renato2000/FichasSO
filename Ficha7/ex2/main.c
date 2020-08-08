#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int file_nr = argc-2;
	int pids[file_nr];
	int status, find = 1;

	for(int i = 0; i < file_nr; i++){
		if((pids[i] = fork()) == 0){
			int ret = execlp("grep", "grep", argv[1], argv[i+2], NULL);
			_exit(-1);
		}
	}

	for(int i = 1; i <= file_nr && find; i++){
		int terminated_pid = wait(&status);

		if(WIFEXITED(status)){
			switch(WEXITSTATUS(status)){
				case 0:
					printf("[PID] %d encontrou a palavra.\n", terminated_pid);
					find = 0;
					for(int j = 0; j < file_nr; j++)
						if(pids[j] != terminated_pid && pids[i] > 0){
							kill(pids[j], SIGKILL);
						} 
					break;
				case 1: 
					printf("[PID] %d não encontrou a palavra.\n", terminated_pid);
					break;
			}
		}
	}
	return 0;
}
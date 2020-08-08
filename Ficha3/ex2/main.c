#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	int fork_ret, exec_ret, wait_ret, status;
	char *exec_args[] = {"bin/ls", "-l", NULL};

	if((fork_ret = fork()) == 0){
		exec_ret = execv("/bin/ls", exec_args);
		perror("reached return");
		_exit(exec_ret);
	}
	else{
		wait_ret = wait(&status);
		if(WIFEXITED(status)){
			printf("O filho returnou %d\n", WEXITSTATUS(status));
		}
		else{
			printf("O filho não terminou\n");
		}
	}

	return 0;
}
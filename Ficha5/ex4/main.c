#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

//ls /etc | wc -l
int main(int argc, char* argv[]){
	int p[2];
	int pid1, pid2;

	if(pipe(p) == -1){
		perror("pipe");
		return -1;
	}

	if((pid1 = fork()) == 0){
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		int ret = execlp("ls", "ls", "/etc", NULL);
		_exit(ret);
	}
	else{
		close(p[1]);
		if((pid2 = fork()) == 0){
			dup2(p[0], 0);
			close(p[0]);
			int ret = execlp("wc", "wc", "-l", NULL);
			_exit(ret);
		}
		else{
			close(p[0]);
			wait(NULL);
			wait(NULL);
		}
	}
	return 0;
}
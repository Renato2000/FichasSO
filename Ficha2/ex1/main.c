#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	pid_t pai = getppid();
	pid_t filho = getpid();
	printf("Processo pai: %d\nProcesso filho: %d\n", pai, filho);
	return 0;
}
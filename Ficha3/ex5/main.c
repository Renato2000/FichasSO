#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	int exec_ret, fork_ret, i;
	for(int i=1; i<argc; i++){
		fork_ret = fork();
		if(fork_ret == 0){
			printf("Sou o filho %d e vou executar o comando %s\n", getpid(), argv[i]);
			exec_ret = execlp(argv[i], argv[i], NULL);
			perror("reached return");
			_exit(exec_ret);
		}
	}

	for(int i=1; i<argc; i++){
		printf("Sou o pai e estou à espera dos filhos\n");
		wait(NULL);
	}
	printf("todos os filhos executaram\n");

	return 0;
}
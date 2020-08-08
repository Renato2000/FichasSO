#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
	if(argc < 2){
		printf("Usage: program <needle>\n");
		exit(-1);
	}

	pid_t pid;

	int needle = atoi(argv[1]);
	int rows = 10;
	int cols = 10000;
	int rand_max = 10000;
	int status;
	int **matrix;

	//Allocate and populate matrix with random nunbers.
	printf("Generatng numbers from 0 to %d...\n", rand_max);
	matrix = (int **) malloc(sizeof(int *) * rows);
	for(int i = 0; i < rows; i++){
		matrix[i] = (int *) malloc(sizeof(int) * cols);
		for(int j = 0; j < cols; j++){
			matrix[i][j] = rand() % rand_max;
		} 
	}
	printf("Done.\n");

	//Procura pelo número na matriz
	for(int i = 0; i < rows; i++){
		if((pid = fork()) == 0){
			printf("[pid %d] row: %d\n", getpid(), i);

			//Start searching for the giver nunber in row #i
			for(int j = 0; j < cols; j++){
				if(matrix[i][j] == needle)
					_exit(i);
			}
			_exit(-1);
		}
	}

	for(int i = 0; i < rows; i++){
		int terminated_pid = wait(&status);

		if(WIFEXITED(status)){
			if(WEXITSTATUS(status) < 255) //O valor é devolvido em 8 bits -> O 255 corresponde a -1
				printf("[pai] process %d exited. found nunber at row: %d\n", terminated_pid, WEXITSTATUS(status));
			else printf("[pai] process %d exited. nothing found\n", terminated_pid);
		} else{
			printf("[pai] process %d exited. something went wrong\n", terminated_pid);
		}
	}

	return 0;
}
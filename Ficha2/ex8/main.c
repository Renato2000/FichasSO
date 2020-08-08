#include <unistd.h>
#include <fcntl.h>
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
	int pids[rows];

	int fd = open("matriz", O_WRONLY, O_CREAT, 0644);

	//Allocate and populate matrix with random nunbers.
	printf("Generatng numbers from 0 to %d...\n", rand_max);
	
	for(int i = 0; i < rows; i++){
		
		for(int j = 0; j < cols; j++){
			int value = rand() % rand_max;
			

			write(fd, &value, sizeof(int));
		} 
	}
	close(fd);
	printf("Done.\n");

	for(int i = 0; i < rows; i++){
		if((pid = fork()) == 0){

			fd = open("matriz", O_RDONLY);

			printf("[pid %d] row: %d\n", getpid(), i);

			lseek(fd, i*cols*sizeof(int), SEEK_SET);

			//Start searching for the giver nunber in row #i
			for(int j = 0; j < cols; j++){
				int value;

				read(fd, &value, sizeof(int));
				printf("%d\n", value);
				if(value == needle)
					close(fd);
					_exit(i);
			}
			close(fd);
			_exit(-1);
		}
		else{
			pids[i] = pid; //Guarda o pid do filho
		}
	}

	for(int i = 0; i < rows; i++){
		pid_t terminated_pid = waitpid(pids[i], &status, 0);

		if(WIFEXITED(status)){
			if(WEXITSTATUS(status) < 255) //O valor é devolvido em 8 bits -> O 255 corresponde a -1
				printf("[pai] process %d exited. found number at row: %d\n", terminated_pid, WEXITSTATUS(status));
			else printf("[pai] process %d exited. nothing found\n", terminated_pid);
		} else{
			printf("[pai] process %d exited. something went wrong\n", terminated_pid);
		}
	}

	return 0;
}
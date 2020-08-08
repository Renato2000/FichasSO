#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int i;
	for(i=1; argv[i]; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'i':{
					int fd = open(argv[++i], O_RDONLY);
					dup2(fd, 0);
					close(fd);
					break;
				}
				case 'o':{
					int fd = open(argv[++i], O_CREAT | O_WRONLY, 0600);
					dup2(fd, 1);
					close(fd);
					break;
				}
				default:
					printf("Opção inválida.\n");
					break;
			}
		}
		else{
			execvp(argv[i], argv + i);
		}
	}
	return 0;
}
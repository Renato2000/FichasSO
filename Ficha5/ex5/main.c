#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define TAM 1024

//grep -v ˆ# /etc/passwd | cut -f7 -d: | uniq | wc -l

int main(int argc, char* argv[]){
	int n = 4;
	int p[n-1][2];

	char** commands[4];
    char* grep[] = { "grep", "-v", "^#", "/etc/passwd", NULL };
    char* cut[] = { "cut", "-f7", "-d:", NULL };
    char* uniq[] = { "uniq", NULL };
    char* wc[] = { "wc", "-l", NULL };
    commands[0] = grep;
    commands[1] = cut;
    commands[2] = uniq;
    commands[3] = wc;

    for(int i=0; i<n; i++){
    	if(i < n-1){
    		if(pipe(p[i]) == -1){
				perror("pipe");
				return -1;
			}
    	}
    	if(!fork()){
    		if(i > 0){ //Na primeira iteração não existe um pipe anterior
    			dup2(p[i-1][0], 0);
    			close(p[i-1][0]); //fecha o descritor de leitura do pipe anterior
    		}
    		if(i < n-1){ //Na ultima iteração nao copiamos o stdin para o pipe de escrita
    			close(p[i][0]); //Fecha o descritor de leitura
    			dup2(p[i][1], 1);
    			close(p[i][1]); //Fecha o descritor de escrita
    		}
    		execvp(commands[i][0], commands[i]);
    		_exit(1);
    	}
    	else{
    		wait(NULL);
    	}
    	if(i < n-1){ //Na ultima iteração não temos de fechar o pipe
    		close(p[i][1]);
    	}
    	if(n > 0){
    		close(p[i-1][0]);
    	}
    	p[i-1][0] = p[i][0];
    }
    return 0;
}

/*
int main(int argc, char* argv[]){
	int p1[2];

	int STDIN, STDOUT;
	STDIN = dup(0);
	STDOUT = dup(1);

	if(pipe(p1) == -1){
		perror("pipe");
		return -1;
	}

	if(fork() == 0){
		close(p1[0]); //Fecha o descritor de leitura
		dup2(p1[1], 1);
		close(p1[1]);
		int ret = execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);
		_exit(ret);
	}
	else{
		close(p1[1]);
		wait(NULL);

		dup2(1, STDOUT);
		dup2(0, STDIN);

		int p2[2];
		if(pipe(p2) == -1){
			return -1;
		}

		if(!fork()){
			close(p2[0]);

			dup2(p1[0], 0);
			close(p1[0]);
			
			dup2(p2[1], 1);
			close(p2[1]);

			int ret = execlp("cut", "cut", "-f7", "-d:", NULL);
			_exit(ret);
		}
		else{
			close(p1[0]);
			close(p2[1]);
			wait(NULL);

			dup2(1, STDOUT);
			dup2(0, STDIN);

			int p3[2];
			if(pipe(p3) == -1){
				return -1;
			}

			if(!fork()){
				close(p3[0]);

				dup2(p3[1], 1);
				close(p3[1]);

				dup2(p2[0], 0);
				close(p2[0]);

				int ret = execlp("uniq", "uniq", NULL);
				_exit(ret);
			}
			else{
				close(p2[0]);
				close(p3[1]);
				wait(NULL);

				dup2(1, STDOUT);
				dup2(0, STDIN);

				if(!fork()){
					dup2(p3[0], 0);
					close(p3[0]);
					
					int ret = execlp("wc", "wc", "-l", NULL);
					_exit(ret);
				}

				else{
					close(p3[0]);
					wait(NULL);
				}
			}
		}
	}
	return 0;
}
*/
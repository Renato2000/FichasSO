#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define MAX 1024

int main(int argc, char* argv[]){
	char buff[MAX];
	int bytes_read;
	int fd, log, fifo;

	//Abre o ficheiro
	if((log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1){
		perror("Open");
		return -1;
	}

	//Cria o pipeline
	if((fifo = mkfifo("fifo", 0666)) == -1){
		perror("Fifo");
	}

	while(1){
		/*
			Abre o fifo
			
			O fifo tem de ser aberto dentro do ciclo para que o while não detete EOF infinitamente (espera ativa) 
			sempre que um cliente sai do servidor
		*/
		if((fd = open("fifo", O_RDONLY)) == -1){
			perror("Fifo");
			return -1;
		}
		else{
			printf("Opened fifo for writing\n");
		}

		//Lê as mensagens dos clientes
		while((bytes_read = read(fd, buff, MAX)) > 0){
			if(write(log, buff, bytes_read) == -1){
				perror("Write");
			}
		}
		if(bytes_read == 0){
			printf("EOF\n");
		}
		else{
			perror("Read");
		}

		close(fd);
	}

	unlink("fifo");

	return 0;
}
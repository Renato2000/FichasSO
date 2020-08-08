#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

unsigned int time = 0;
unsigned int count = 0;

void alarmS(int signum){
	alarm(1);
	time++;
}

void quitS(int signum){
	printf("\nYou checked the timer %d times,\n", count);
	_exit(0);
}

void intS(int signum){
	printf("\n%ds has passed.\n", time);
	count++;
}

int main(){
	printf("[PID] %d\n", getpid());

	signal(SIGINT, intS); //CTRL-C.
	signal(SIGQUIT, quitS); //CRTL-\.
	signal(SIGALRM, alarmS);

	alarm(1);

	while(1){
		//Se pusser o alarm aqui dentro ele podia resetar 
		//ao receber um sinal qualquer(um spam de sinais), 
		//nunca passando o tempo.
		pause();
		//printf("The clock is ticking...\n");
	}

	return 0;
}
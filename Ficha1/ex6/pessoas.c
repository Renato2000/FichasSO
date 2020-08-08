#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FILENAME "Pessoas.txt"

typedef struct Person{
	char name[200];
	int age;
} Person;

int new_person(char*, int);
int find_pos(char*, int);
int person_change_age(char*, int);
int person_change_age_v2(long pos, int age);
void print_persons();

void main(int argc, char* argv[]){
	int r;
	if(strcmp("-i", argv[1]) == 0){
		r = new_person(argv[2], atoi(argv[3]));
	}
	else if(strcmp("-u", argv[1]) == 0){
		r = person_change_age(argv[2], atoi(argv[3]));
	}	
	else if(strcmp("-I", argv[1]) == 0){
		r = find_pos(argv[2], atoi(argv[3]));
		char text[100];
		sprintf(text, "A pessoa encontra-se no indice: %d\n", r);
		int i;
		for(i=0; text[i] != '\n' && i<99; i++);
		if(i == 99) text[i] = '\n';
		else i++;
		write(1, text, i);
	}	
	else if(strcmp("-U", argv[1]) == 0){
		r = person_change_age_v2(atof(argv[2]), atoi(argv[3]));
	}	
	else if(strcmp("-l", argv[1]) == 0){
		print_persons();
	}	
}

int new_person(char* name, int age){
	int fd = open(FILENAME, O_CREAT | O_WRONLY | O_APPEND, 0600);
	Person p;
	p.age = age;
	strcpy(p.name, name);
	int r = write(fd, &p, sizeof(Person));
	return r;
}

int find_pos(char* name, int age){
	int fd = open(FILENAME, O_RDONLY);
	int r, i=0;
	Person p;
	r = read(fd, &p, sizeof(Person));
	while(strcmp(p.name, name) && r>0){
		r = read(fd, &p, sizeof(Person));
		i++;
	}
	close(fd);
	if(r == 0) return -1;
	return i;
}

int person_change_age(char* name, int age){
	int fd = open(FILENAME, O_RDWR);
	int r;
	Person p;
	r = read(fd, &p, sizeof(Person));
	while(strcmp(p.name, name) && r>0)
		r = read(fd, &p, sizeof(Person));
	if(r == 0) return 0;
	p.age = age;
	lseek(fd, -sizeof(Person), SEEK_CUR);
	write(fd, &p, sizeof(Person));
	close(fd);
	return 1;
}


int person_change_age_v2(long pos, int age){
	int fd = open(FILENAME, O_RDWR);
	int r;
	Person p;
	lseek(fd, pos * sizeof(Person), SEEK_SET);
	r = read(fd, &p, sizeof(Person));
	p.age = age;
	lseek(fd, -sizeof(Person), SEEK_CUR);
	write(fd, &p, sizeof(Person));
	close(fd);
	return 1;
}


void print_persons(){
	int fd = open(FILENAME, O_RDONLY);
	int r;
	Person p;
	r = read(fd, &p, sizeof(Person));
	while(r > 0){
		printf("%s %d\n", p.name, p.age);
		r = read(fd, &p, sizeof(Person));
	}
	close(fd); 
}
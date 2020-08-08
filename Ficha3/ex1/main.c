#include <unistd.h>

int main(int argc, char* argv[]){
	int ret = execl("/bin/ls", "/bin/ls", "-l", NULL);
	return 0;
}
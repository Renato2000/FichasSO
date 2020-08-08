#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
	int exec_ret;
	char* exec_args[] = {"argumento 0", "argumento 1", "argumento 2", NULL};
	exec_ret = execv("../ex3/program", exec_args);
	return 0;
}
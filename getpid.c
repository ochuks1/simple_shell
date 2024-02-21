#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;
	pid = getpid();
	ppid = getppid();
	printf("pid = %d\nparent pid= %d\n", pid, ppid);
	return 0;
}

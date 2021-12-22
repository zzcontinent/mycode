#include "stdio.h"
#include "unistd.h"

int main(int argc, char * argv[])
{
	printf("ruid:%d\n", getuid());
	printf("euid:%d\n", geteuid());

	if (execvp(argv[1], argv+1) == -1)
	{
		perror("execvp error!\n");
	}
	return 0;
}

/*
 * sudo chown root a.out
 * sudo chmod +s a.out
 * ./a.out apt update
 * */

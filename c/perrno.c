#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
	int start = 0;
	int len = 0;
	if (argc > 2 || (argc == 2&& !isdigit(*argv[1])))
	{
		printf("Usage:\n"
				"perrno     : print total 200 errno\n"
				"perrno 123 : print errno=123\n");
		return 1;
	}

	if (argc == 1)
	{
		len = 200;
	} else if (argc == 2  ){
		start = atoi(argv[1]);
		len = start + 1;
	}

	printf("+---------------------------------------------------------+\n");
	for (int i=start; i<len; i++)
	{
		printf("|%-4d | %-50s|\n", i, strerror(i));
		if (i == len-1)
			printf("+---------------------------------------------------------+\n");
		else
			printf("|---------------------------------------------------------|\n");
	}
	return 0;
}

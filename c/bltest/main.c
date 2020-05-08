#include <stdio.h>

int main()
{
	int x = 0x1234;
	char * p = (char *)&x;
	printf("%x,%x\n",(int)p[0],(int)p[1]);
	if ( p[0] == 0x34 )
	{
		printf("little endian\n");
	}
	else
	{
		printf("big endian\n");
	}
	return 0;
}

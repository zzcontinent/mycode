#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void main(int argc, char* argv[])
{
	if (argc <=1)
	{
		printf("input 1 number");
		return;
	}
	
	int in[4];
	in[0]=atoi(argv[1]);

	for (int i=0;i<in[0];i++)
	{
		for(int ii=0;ii<i;ii++)
		printf("%d/%d =  %d%%\n",i,in[0],i*100/in[0]);
	}

}

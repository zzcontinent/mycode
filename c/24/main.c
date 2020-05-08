#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void iterSeq(int* in, int cnt)
{
	//0 1 2 3
	//0 1 3 2
	//0 2 1 3
	//0 2 3 1
	//0 3 1 2
	//0 3 2 1

	//1 0 2 3
	//1 0 3 2
	//1 2 0 3
	//1 2 3 0
	//1 3 0 2
	//1 3 2 0

	for (int i = cnt- 1;i>0;i--)
	{
		for( int j=i-1;j>=0;j--)
		{
			if (in[i] > in[j])
			{
				int tmp = in[i];
				in[i] = in[j];
				in[j] = tmp;
				sort(in+j+1, cnt-j-1);
				return;
			} 
		}
	}
}

void sort(int* in, int cnt)
{
	for(int i = 0;i<cnt-1;i++)
	{
		for(int j=i+1;j<cnt;j++)
		{
			if (in[i] < in[j])	
			{
				int tmp=in[i];
				in[i] = in[j];
				in[j] = tmp; 
			}
		}
	}
	for (int i = 0;i<cnt/2;i++)
	{
		int tmp = in[i];
		in[i] = in[cnt - 1 -i];	
		in[cnt - 1 - i] = tmp;
	}
}

int getmin_pos(int* in, int cnt)
{
	if (cnt == 0)
		return 0; 

	int min_pos  = 0;
	for (int i = 0;i<cnt;i++)
	{
		if(in[i] < in[min_pos])
		{
			min_pos = i;
		}
	}
	return min_pos;
}

int op(int in1, int in2, int op)
{
	switch (op){
	case 0:
		return in1 + in2;
	case 1:
		return in1 - in2;
	case 2:
		return in1 * in2;
	case 3:
		if ( in1 % in2 != 0)
			return 99999999;
		else
			return in1 / in2;
	}
}

char itoa_op(int op)
{
	switch (op){
	case 0:
		return '+';
	case 1:
		return '-';
	case 2:
		return 'x';
	case 3:
		return '/';
	}
}


void main(int argc, char* argv[])
{
	if (argc <=1 || argc !=5)
	{
		printf("input 4 number");
		return;
	}
	
	int in[4];
	in[0]=atoi(argv[1]);
	in[1]=atoi(argv[2]);
	in[2]=atoi(argv[3]);
	in[3]=atoi(argv[4]);
	sort(in,4);
	for (int i=0;i<4*3*2;i++)
	{
		for(int ii=0;ii<4;ii++)
			for(int iii=0;iii<4;iii++)
				for(int iiii=0;iiii<4;iiii++)
				{
					int val1 = op(in[0],in[1],ii);
					int val2 = op(val1,in[2],iii);
					int val3 = op(val2,in[3],iiii);
					if (val3 == 24)
					{
						printf("=======================%d %c %d %c %d %c %d= %d\n",in[0],itoa_op(ii),in[1],itoa_op(iii),in[2],itoa_op(iiii),in[3],val3);
					}
				}
		iterSeq(in, argc-1);
		printf("------------------%d %d %d %d\n",in[0],in[1],in[2],in[3]);
	}

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "libx.h"

struct cs
{
	int a;
	int b;
};

struct cs testcs = {
	.a=1,
	.b=2,
};

void print()
{
    int num = 1;
    num += 1;
    printf("Num = %02d\n",num);
}

void test()
{
    int a[4];
    int b[4];
    strcpy((char*)a,"ABCDEFGHIJKLMN");
    strcpy((char*)b,"12345678");
    printf("%s",(char*)a);
    printf("%ld",strlen((char*)a));
    printf("%ld",sizeof(a)/sizeof(a[0]));
    printf("%s",(char*)b);
    printf("%ld",strlen((char*)b));
    printf("%ld",sizeof(b)/sizeof(b[0]));
}
int testmem()
{
 	int a,b;
	a=100;
	b=1;
	b += a;
	return b;
}
void test_watch()
{
	testcs.a += 1;	
	testcs.b += 1;	
}

void test_len()
{
	int a[10];
	struct x{
		int a;
		int b;
	}b;
	printf("%d,%d\n",sizeof(a),sizeof(b));
	printf("%d\n",sizeof(a)/sizeof(a[0]));
}


int main()
{
//	int fd;
//	fd = open("/home/cliff/tmp.txt",O_RDWR);
//	printf("%d",fd);
//	printf("%d\n",testcs.b);
//	printf("%d\n",testcs.a);
//	printf("%d\n",libx_test());
//	test_watch();

//	printf("%s\n",xxx);
	int b = 10;
	u_int32_t* a = &b;
	int c = a;
	printf("%x\n",sizeof(&b));
	printf("%x\n",sizeof(a));
	printf("%x\n",&b);
	printf("%x\n",c);
	printf("%x\n",a);
	printf("%x\n",1<<2);
	return 0;
// return testmem();
}





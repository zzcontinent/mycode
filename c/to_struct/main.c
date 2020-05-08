#include "stdio.h"

typedef  struct{
	int a;
	char b;
	char * c;
}__test;

#define offsetof(type, member) \
	((size_t)(&(((type *)0)->member)))
#define to_struct(ptr, type, member) \
	((type *)((char *)ptr - offsetof(type, member)))


void main()
{
	printf("%d\n",sizeof(__test));
	printf("%d\n",offsetof(__test, c));
	__test test1 = {1,1,"123"};
	printf("%d\n",to_struct(&test1.c, __test, c));
	printf("%d\n",&test1);
}

#include <stdio.h>

union BitTest {
	unsigned int v;
	struct {
		unsigned a:3;
		unsigned b:1;
		unsigned c:4;
	} bits;
};

int main(void)
{
	union BitTest test= {.v = 0};
	test.bits.a = 7;
	test.bits.b = 0;
	test.bits.c = 15;
	printf("%x\n", test.v);
}

#include <stdio.h>

union reg1 {
	unsigned char v;
	struct {
		unsigned a:3;
		unsigned b:1;
		unsigned c:4;
	} bits;
};

struct AllReg {
	union reg1 reg11;
	union reg1 reg22;
};

int main(void)
{
	struct AllReg test= { .reg11 = {.bits.a = 7, .bits.b=0, .bits.c = 15}, .reg22 = { .bits.a = 7, .bits.b=0, .bits.c=15}};
	printf("%x\n", test.reg11.v);
	printf("%x\n", test.reg22.v);
}

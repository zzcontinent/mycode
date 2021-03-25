#include <stdio.h>

int isBigEnd()
{
    int x = 0x1234;
    char* p = (char*)&x;
    printf("%x,%x\n", (int)p[0], (int)p[1]);
    if (p[0] == 0x34) {
	printf("little endian\n");
	return 0;
    } else {
	printf("big endian\n");
	return 1;
    }
}

int main()
{
    printf("%d", isBigEnd());
    return 0;
}

#include <stdio.h>

char* strcpy(char *dst, const char *src)
{
	__asm__("cld\n"
		"1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b"
		:
		:"S" (src), "D" (dst)
		:"ax");
	return dst;
}

int main(int argc, char *argv[])
{
	char buf[512];

	strcpy(buf,"Hello,AT&T!");
	printf("%s\n", buf);
	return 0;
}

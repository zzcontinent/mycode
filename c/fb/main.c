#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>

static void *fbp = 0;
int fbfd = 0;
int fbsize = 800*600;

int main()
{
	int i;
	fbfd = open("/dev/fb0", O_RDWR);
	fbp = mmap(0, fbsize*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (fbp == MAP_FAILED)
	{
		printf("Error: failed to map framebuffer device to memory.%d,%s\n",(int)fbp,strerror(errno));
		return -1;
	}
	int *add;
	add = (int *)fbp;
	for(i=0; i< fbsize; i++)
	{
		*add = 0x000000ff;
		add++;
	}
	munmap((void *)fbp,fbsize*sizeof(int));
	return 0;
}


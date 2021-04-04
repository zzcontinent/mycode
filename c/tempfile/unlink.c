#include <stdio.h>
#include <errno.h>
#include <fcntl.h>


int main() {
	char a[10]={0};
	int fd = open("./123", O_CREAT|O_RDWR);
	if (fd == -1)
	{
		printf("error no: %d", errno);
		exit(1);
	}
	printf("---%d\n",write(fd, "123456", 7));
	unlink("./123");
	lseek(fd, 0, SEEK_SET);
	printf("---%d\n",read(fd,a,7));
	printf("---%s\n", a);
}

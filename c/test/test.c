#include <unistd.h>
#include "stdio.h"
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "utils.h"

#define SYSFS_GPIO_EXPORT "/sys/class/gpio/gpio135/value"

void main()
{
	printf("main\n");
	char buf[10240]={0};
	execmd("ifconfig",buf);
	printf(buf);
	return;
/*	
	int fd = open(SYSFS_GPIO_EXPORT, O_RDONLY);
	if(fd == -1)
	{
		printf("open error.\n");
		return ;
	}
*/

	char buffer[10]={0};
	int len;
	char val='\0';
	for(;;){
		
		int fd = open(SYSFS_GPIO_EXPORT, O_RDONLY);
		if(fd == -1)
		{
			printf("open error.\n");
			return ;
		}
		
		len = read(fd, buffer, sizeof(buffer)); 
		if (len == -1){
			printf("read error\n");
			return;
		}
		//printf("buf:%s\n",buffer);
		if (buffer[0] != val) {
			printf("diff:%s\n",buffer);
			val = buffer[0];
		}
		usleep(1000);
		close(fd); 
	}
	//close(fd); 
}


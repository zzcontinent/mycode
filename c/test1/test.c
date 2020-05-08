#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include "utils.h"


void main()
{
	printf("main\n");

	char buffer[10]={0};
	int len;
	char val='\0';


	for(int i=0; i< 20; i++){
		printf("%d\n", i);
		void * pn = malloc(512 * 1024 * 1024);
		memset(pn,255, 512 * 1024 * 1024);
		
		printf("amlloc addr: %d\n", pn);
		printf("amlloc: %d\n", *((char*)pn + 512*1024*1024-1));
		printf("amlloc errno: %d\n", errno);

		pid_t pid=fork();
		if ( pid < 0 ) {
			printf("errno: %d\n", errno);
		} else if( pid == 0 ) {
			printf("child\n");
			sleep(100);
			return;
		}
		sleep(5);
	}
}


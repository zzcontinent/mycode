#include <stdio.h>
#include <sys/syscall.h>
#include <syscall.h>

int main(void)
{
	if (fork() == 0)
	{
		if (fork() == 0)
		{
			write(1, "1---hello\n", 11);
			sleep(5);
		} else{
			wait();
			write(1, "2---world\n", 11);
			sleep(5);
		}
	} else{
		wait();
		write(1, "3---now\n", 9);
	}
}

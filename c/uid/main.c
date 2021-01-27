#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    setuid(0);
    printf("pid=%d\n", getpid());
    printf("ppid=%d\n", getppid());
    printf("uid=%d\n", getuid());
    printf("euid=%d\n", geteuid());
    printf("sid=%d\n", getsid(getpid()));
    int pid1 = system("fdisk -l > ./1");
    printf("pid1=%d\n", pid1);
    wait(&pid1);
}

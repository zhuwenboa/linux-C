#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
	int fd;
	printf("uid study: \n");
	printf("Process's uid = %d, euid = %d\n", getuid(), geteuid());
	if(fd = open("test.c", O_RDWR) == -1)
	{
		perror("creat file failed");
		exit(0);
	}
	else
	{
		printf("Open successfully\n");
	}
	close(fd);
	exit(0);
}

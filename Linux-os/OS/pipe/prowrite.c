#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>

int main(void)
{
	int fd;
	char buf[1024] = "Hello procwrite, I come from process named procread!";
	umask(0);
	if(mkfifo("myfifo", S_IFIFO | 0666) == -1)
	{
		perror("mkfifo failed");
		exit(0);
	}
	if( (fd = open("myfifo", O_WRONLY)) == -1)
	{
		perror("fopen error");
		exit(0);
	}
	write(fd, buf, strlen(buf) + 1);
	close(fd);
	exit(0);
}

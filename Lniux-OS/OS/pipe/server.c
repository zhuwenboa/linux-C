#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<errno.h>

int main(void)
{
	int wfd, rfd;
	char writebuf[1024] = {0};
	char readbuf[1024] = {0};
	int len;
	if(mkfifo("writefifo", S_IFIFO | 0666))
	{
		printf("Can't create FIFO writefifo because %s\n", strerror(errno));
		exit(0);
	}
	if((wfd = open("writefifo", O_WRONLY)) == -1)
	{
		printf("Fail open FIFO %s\n", strerror(errno));
	}
	while( (rfd = open("readfifo", O_RDONLY)) == -1)
		sleep(1);
	while(1)
	{
		printf("PXZ: ");
	//	scanf("%[^\n]", writebuf);
		fgets(writebuf, 1024, stdin);
		writebuf[strlen(writebuf) - 1] = '\0';
		if(strncmp(writebuf, "quit", 4) == 0)
		{
			close(wfd);
			unlink("writefifo");
			close(rfd);
			exit(0);
		}
		write(wfd, writebuf, strlen(writebuf));
		len = read(rfd, readbuf, 1024);
		if(len > 0)
		{
			readbuf[len] = '\0';
			printf("ZWb: %s\n", readbuf);
		}
		memset(writebuf, '\0', 1024);
		memset(readbuf, '\0', 1024);
	}
}

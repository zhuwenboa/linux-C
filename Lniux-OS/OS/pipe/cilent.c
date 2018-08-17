#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>

int main()
{
	int wfd, rfd;
	char writebuf[1024] = {0};
	char readbuf[1024] = {0};
	int len;
	if(mkfifo("readfifo", S_IFIFO | 0666))
	{
		printf("Cat't make readfifo because %s\n", strerror(errno));
		exit(0);
	}
	while((rfd = open("writefifo", O_RDONLY)) == -1)
	{
		sleep(1);
	}
	if((wfd = open("readfifo", O_WRONLY)) == -1)
	{
		printf("Fail open FiFO%s\n", strerror(errno));
		exit(0);
	}
	while(1)
	{
		len = read(rfd, readbuf, 1024);
		if(len > 0)
		{
			readbuf[len] = '\0';
			printf("pxz: %s\n", readbuf);
		}
	//	scanf("%[^\n]", writebuf);
		printf("Zwb: ");
		fgets(writebuf, 1024, stdin);
		writebuf[strlen(writebuf) - 1] = '\0';
		if(strncmp(writebuf, "quit", 4) == 0)
		{
			close(wfd);
			unlink("readfifo");    
			close(rfd);
			exit(0);
		}
		write(wfd, writebuf, 1024);
		memset(readbuf, '\0', 1024);
		memset(writebuf, '\0', 1024);
	}

}

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main()
{
	int fd;
	if((fd = open("1.c", O_CREAT|O_EXCL, S_IWUSR|S_IRUSR)) == -1)           //返回-1代表文件打开失败
	{
		//if((fd = creat("1.c", S_IRWXU)) == -1)
		perror("open");
		//printf("open:%s  with errno:%d\n", strerror(errno), erro);
		exit(1);
	}
	else
	{
		printf("creatfile success\n");
	}
	close(fd);
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>

void display_time(const char *string)
{
	int seconds;
	seconds = time((time_t *)  NULL);
	printf("%s, %d\n", string, seconds);
}

int main(void)
{
	fd_set readfds, writefds;
	struct timeval timeout;
	int ret;
	/*监视文件描述符0是否有数据输入，文件描述符0表示标准输入*/
	FD_ZERO(&readfds);       //将文件描述符集合set清空
	FD_ZERO(&writefds);
	FD_SET(1, &writefds);
	FD_SET(0, &readfds);                                  //在文件描述符中增加文件描述符fd

	/*设置阻塞时间为10秒*/
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	while(1)
	{
		display_time("before select");
		ret = select(1, &readfds, &writefds, NULL, &timeout);
		display_time("after select");
		switch((FD_ISSET(0, readfds)))
		{
			case 0:
				printf("No data in ten seconds\n");
				exit(0);
				break;
			case -1:
				perror("select");
				exit(1);
				break;
			default:
				printf("Data is available now.\n");
				break;
		}
	}
	return 0;
}

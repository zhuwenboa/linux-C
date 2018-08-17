#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

//实现父进程和子进程交替执行

int main(void)
{
	pid_t pid;
	char *msg;
	int k;
	printf("creat fork\n");
	pid = fork();
	switch(pid)
	{
		case 0:
			msg = "child process is running";
			k = 3;
			break;
		case -1:
			perror("Process creat fault");
			break;
		default:
			msg = "Parent process is running";
			k = 5;
			break;
	}
	while(k > 0)
	{
		puts(msg);
		sleep(1);
		k--;
	}
	exit(0);
}

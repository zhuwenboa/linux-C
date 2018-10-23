#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	int num = 5;
	int status;
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("create porcess is failed");
			exit(-1);
		case 0:
		    num = 10;
			printf("child process num is:%d\n", num);
			exit(0);
		default:
			wait(&status);
			printf("parent process num is: %d\n", num);
			break;
	}
	return 0;
}

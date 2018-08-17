#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

//父进程和子进程执行顺序由内核算法调度。

int main(void)
{
	pid_t pid;
	printf("creat fork\n");
	pid = fork();
	switch(pid)
	{
		case 0:
			printf("child process is running, Curpid is %d, Parentpid is %d\n", pid, getppid());
			break;
		case -1:
			perror("Process creat fault");
			break;
		default:
			printf("Parent process is running, Childpid is %d, Parentpid is %d\n", pid, getppid());
			break;
	}
}

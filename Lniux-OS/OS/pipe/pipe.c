#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

void read_pipe(int fd)
{
	char message[100];
	if(read(fd, message, 100) == -1)
	{
		perror("read erro");
		exit(0);
	}
	printf("%s\n", message);
}

void write_pipe(int fd)
{
	char *message = "Hello, pipe!";
	if( write(fd, message, strlen(message + 1)) == -1)
	{
		perror("write defeat");
		exit(0);
	}
}

int main()
{
	int fd[2];
	pid_t pid;
	int stat_val;
	if(pipe(fd))
	{
		perror("fd is failed");
		exit(0);
	}
	pid = fork();
	switch(pid)
	{
		case -1:
			printf("fork failed\n");
			exit(-1);
		/*子进程关闭写端*/
		case 0:
			close(fd[1]);
			read_pipe(fd[0]);
			exit(0);
		/*父进程关闭读端*/
		default:
			close(fd[0]);
			write_pipe(fd[1]);
			waitpid(pid, &stat_val, 0);
			exit(0);
	}
	return 0;
}

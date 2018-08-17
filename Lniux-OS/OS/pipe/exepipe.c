#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char *argv[], char **environ)
{
	int fd[2];
	pid_t pid;
	int status;
	if(argc < 2)
	{
		printf("wrong parameters\n");
		exit(0);
	}
	if(pipe(fd))
	{
		perror("pipe failed");
		exit(0);
	}
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(0);
		case 0:
			close(fd[1]);
			dup(fd[0]);
			execve("exam", (void *)argv, environ);
			exit(0);
		default:
			close(fd[0]);
			write(fd[1], argv[1], strlen(argv[1]));
			break;
	}
	waitpid(pid, &status, 0);
	exit(0);
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void child_rw_pipe(int readfd, int writefd)
{
	char *message1 = "from child pessage";
	write(writefd, message1, strlen(message1) + 1);
	char message2[100];
	read(readfd, message2, 100);
	printf("child read: %s\n", message2);
}
void parent_w_pipe(int writefd)
{
	char *message1 = "from parent message";
	write(writefd, message1, strlen(message1) + 1);
}
void parent_r_pipe(int readfd)
{
	char message[100];
	read(readfd, message, 100);
	printf("parent read: %s\n", message);
}

int main()
{	
	pid_t pid;
	int status;
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(0);
		case 0:
			close(fd1[1]);
			close(fd2[0]);
			child_rw_pipe(fd1[0], fd2[1]);
			exit(0);
		default:
			close(fd1[0]);
			close(fd2[1]);
			parent_w_pipe(fd1[1]);
			waitpid(pid, &status, 0);
			parent_r_pipe(fd2[0]);
			exit(0);
	}
	return 0;
}

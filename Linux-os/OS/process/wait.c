#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void)
{
	pid_t pid;
	char *msg;
	int k;
	int exit_code;

	printf("Study how to get exit code\n");
	pid = fork();
	switch(pid)
	{
		case 0:
			msg = "Child process is running";
			k = 5;
			exit_code = 37;
			break;
		case -1:
			perror("creat process failed");
			exit(0);
		default:
			exit_code = 0;
			break;
	}
	if(pid != 0)
	{
		int stat_val;
		pid_t child_pid;
		child_pid = wait(&stat_val);
		printf("Child process has exited, pid = %d\n", child_pid);
		if(WIFEXITED(stat_val))
		{
			printf("Child exited code %d\n", WEXITSTATUS(stat_val));
		}
		else 
			printf("Child exited abnormally\n");
	}
	else
	{
		while(k)
		{
			k--;
			puts(msg);
			sleep(2);
		}
	}
	exit(exit_code);
}


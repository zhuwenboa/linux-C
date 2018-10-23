#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

//fork与vfork的区别

int count = 5;

int main(void)
{
	pid_t pid;
	int num = 1;
	//pid = fork();
    pid = vfork();
  	int i;
  	switch(pid)
	{
		case 0:
			i = 4;
			while(i-- > 0)
			{
				printf("child process is running\n");
				count++;
				num++;
				sleep(2);
			}
			printf("%p\n", &num);
			printf("child process: num = %d, count = %d\n", num, count);
			exit(0);
		//	break;
		case -1:
			perror("creat procsee fault");
			exit(0);
		default:
			i = 5;
			//sleep(20);
			while(i-- > 0)
			{
				printf("parent process is running\n");
				count++;
				num++;
			//	sleep(2);
			}
			printf("%p\n", &num);
			printf("parent process: num = %d, count = %d\n", num, count);
			_exit(0);
	}
}

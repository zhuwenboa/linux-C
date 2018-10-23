#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(int n, char *argv[], char **environ)
{
	int i;
	printf("I am a process image!\n");
	printf("My pid = %d, gid = %d\n", getuid(), getgid());
	for(i = 0; i < n; i++)
	{
		printf("%d : %s\n", i, argv[i]);
	}
}

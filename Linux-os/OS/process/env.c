#include<stdio.h>
#include<malloc.h>
int main(int argc, char *argv[], char **envp)
{
	int i;
	printf("path:\n");
	for(i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
	printf("Enviroment\n");
	for(i = 0; i < argc; i++)
		printf("%s", envp[i]);
	return 0;
}

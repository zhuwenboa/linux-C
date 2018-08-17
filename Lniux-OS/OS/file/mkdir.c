#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(int argc, char *argv[])
{
	int mode, mode_u, mode_g, mode_o;
	if(argc < 3)
	{
		perror("input erro\n");
		exit(0);
	}
	mode = atoi(argv[2]);
	mode_u = mode / 100;
	mode_g = mode / 10 - mode_u * 10;
	mode_o = mode - mode_u * 100 - mode_g * 10;
	mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;
	if(mkdir(argv[1], mode) == -1)
	{
		perror("mkdir error\n");
		exit(1);
	}
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
        int mode;              //权限
        int mode_u = 0;            //所有者权限
        int mode_g = 0;            //所属用户组权限
        int mode_o = 0;            //其他用户权限
        char *path;
	int i, j;
	int len;
	char str[4];
	struct stat buf;    

	if(argc < 3)
	{
		perror("input erro");
		exit(0);
	}
	
	if(stat(argv[argc - 1], &buf) == -1)
	{
		perror("stat failed");
		exit(0);
	}
	//获取文件权限
	mode = buf.st_mode;
        if(mode & S_IRUSR)
                mode_u += 4;
        if(mode & S_IWUSR)
                mode_u += 2;
        if(mode & S_IXUSR)
                mode_u += 1;
        if(mode & S_IRGRP)
                mode_g += 4;
        if(mode & S_IWGRP)
           	mode_g += 2;
        if(mode & S_IXGRP)
                mode_g += 1;
        if(mode & S_IROTH)
                mode_o += 4;
        if(mode & S_IWOTH)
                mode_o += 2;
        if(mode & S_IXOTH)
                mode_o += 1;
	
	
	for(i = 1; i < argc - 1; i++)
	{	
		len = strlen(argv[i]);
		if(argv[i][0] == 'u')
		{
			if(argv[i][1] == '+')
			{
				for(j = 2; j < len; j++)
				{
					if(argv[i][j] == 'r' && !(mode & S_IRUSR))
					{
						mode_u += 4;
						continue;
					}
					if(argv[i][j] == 'w' && !(mode & S_IWUSR))
					{
						mode_u += 2;
						continue;
					}
					if(argv[i][j] == 'x' && !(mode & S_IXUSR))
					{
						mode_u += 1;
						continue;
					}
				}
			}
		}
	}
	mode = 0;
	mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;
	if(chmod(argv[argc - 1], mode) == -1)
	{
		perror("chmod error");
		exit(0);
	}
        return 0;
}

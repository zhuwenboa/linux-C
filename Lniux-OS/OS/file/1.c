#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>

/*
-a flag = 1；
-l flag = 2;
-la flag = 3;
-R flag = 4;
-aR flag = 5;
-lR flag = 6;
-laR flag = 7;
*/

void print(char *name);
void erro(char *str, int line);
int my_readir(char * path, int flag);

void erro(char *str, int line)                     //错误处理函数
{          
	fprintf(stderr, "line: %d", line);
	perror(str);
   	exit(0);
}          

int my_readir(char * path, int flag)
{
	int i, j;
	int count = 0;
	DIR * dir;
	struct dirent * ptr;
	char name[50][50] = {0};
	struct stat buf;                 
	if(stat(path, &buf) == -1)
		erro("stat", __LINE__);
	if(S_ISDIR(buf.st_mode))
	{	
		if(chdir(path) == -1)
			erro("chdir", __LINE__);
		if((dir = opendir(path)) == NULL)
		{
			perror("opendir");
			return -1;
		}
		while((ptr = readdir(dir)) != NULL)
		{
			strcpy(name[count], ptr->d_name);
			count++;
			//print(ptr->d_name);
		}
		int array[count], temp;
		for(i = 0; i < count; i++)
		array[i] = i;
		/*对文件进行排序*/
		for(i = 0; i < count - 1; i++)
		{
			for(j = 0; j < count - i - 1; j++)
			{
				if(strcmp(name[array[j]], name[array[j + 1]]) > 0)
				{
					temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
		switch(flag)
		{
			case 0:
				for(i = 0, j = 0; i < count; i++)
				{
					if(name[array[i]][0] == '.')
						continue;
					//if(stat(name[array[i]], &buf) == -1)
					//	erro("stat", __LINE__);
					//if(S_ISDIR(buf.st_mode))
					//	printf("%s", 
					printf("%s\t", name[array[i]]);
					j++;
					if((j + 1) % 5 == 0)
						printf("\n");
				}
				printf("\n");
				break;
			case 1:
				for(i = 0; i < count; i++)
				{
					printf("%s\t", name[array[i]]);
					if((i + 1) % 5 == 0)
						printf("\n");
				}
				printf("\n");
				break;
			case 2:
				for(i = 0; i < count; i++)
				{
					if(name[array[i]][0] == '.')
						continue;
					print(name[array[i]]);
				}
					break;
			case 3:
				for(i = 0; i < count; i++)
					print(name[array[i]]);
					break;
		}
	}
	else
	{
		print(path);
	}
	closedir(dir);
	return 0;
}
void print(char * name)
{       
        int mode;
        char buf_time[32];
	struct stat buf;
        struct group *grp;      //获取文件所属用户组名
        struct passwd *pwd;     //获取文件所属用户名
        if(lstat(name, &buf) == -1)
		erro("stat", __LINE__);
        mode = buf.st_mode;
        
        /*获取文件权限*/
        if(S_ISLNK(mode))
                printf("l");
        else if(S_ISREG(mode))
                printf("-");
        else if(S_ISDIR(mode))
                printf("d");
        else if(S_ISCHR(mode))
                printf("c");
        else if(S_ISBLK(mode))
                printf("b");
        else if(S_ISFIFO(mode))
                printf("f");
        else if(S_ISSOCK(mode))
                printf("s");            
        /*获取并打印文件所有者的权限*/
        if(mode & S_IRUSR)
                printf("r");
        else             
                printf("-");
        if(mode & S_IWUSR)
                printf("w");
        else             
                printf("-");
        if(mode & S_IXUSR)
                printf("x");
        else             
                printf("-");
         /*获取并打印文件所属组的权限*/
        if(mode & S_IRGRP)
                printf("r");
        else             
                printf("-");
        if(mode & S_IWGRP)
                printf("w");
        else             
                printf("-");
        if(mode & S_IXGRP)
                printf("x");
        else             
                printf("-");
        
        
        /*获取并打印文件其他用户的权限*/
        if(mode & S_IROTH)                                                                                                                               
                printf("r");
        else             
                printf("-");
        if(mode & S_IWOTH)
                printf("w");
        else             
                printf("-");
        if(mode & S_IXOTH)
                printf("x");
        else             
                printf("-");
                         
       
        printf(" %ld", buf.st_nlink);    //打印文件链接数
        pwd = getpwuid(buf.st_uid);
        grp = getgrgid(buf.st_gid);
        printf(" %s", pwd->pw_name);
        printf(" %s", grp->gr_name);
       
        printf(" %ld", buf.st_size);  //打印文件大小
        strcpy(buf_time, ctime(&buf.st_mtime));      //取出文件的时间
        buf_time[strlen(buf_time) - 1]  = '\0';     //去掉换行符
        printf(" %s", buf_time);   //打印文件时间信息
	printf(" %s\n", name);     //打印文件名字
}


int main(int argc, char * argv[])
{
	int flag = 0;
	int i, j;
	int len;
	char pathname[50] = {0};
	char path[50] = {0};
	for(i = 0; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			len = strlen(argv[i]);
			for(j = 1; j < len; j++)
			{
				if(argv[i][j] == 'a')
				{
					flag += 1;
					continue;
				}
				if(argv[i][j] == 'l')
				{
					flag += 2;
					continue;
				}
				if(argv[i][j] == 'R')
				{
					flag += 4;
					continue;
				}
			}
		}
	}
	printf("flag = %d\n", flag);
	if(argc < 2)
		erro("输入错误", __LINE__);
	else if(argc == 2)
	{
		if(getcwd(pathname, 50) == NULL)
			erro("getcwd", __LINE__);
		my_readir(pathname, flag);
	}
	else if(argc == 3)
	{
		if(flag == 0)
		{
			strcpy(pathname, argv[2]);
			my_readir(pathname, flag);
		}
		else
		{
			if(getcwd(pathname, 50) == NULL)
				erro("getcwd", __LINE__);
			my_readir(pathname, flag);
		}
	}
	else
	{
		if(flag == 0)
			erro("flag", __LINE__);
		else
		{
			strcpy(pathname, argv[argc - 1]);
			my_readir(pathname, flag);
		}
	}

	

	return 0;
}

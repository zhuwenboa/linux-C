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

#define maxline 80                   //一行最大输出字符数

#define CLOSE printf("\033[0m");	 //关闭彩色字体
#define YELLOW printf("\e[1;33m"); 	 //黄色字体
#define GREEN printf("\e[1;32m");	 //绿色字体
#define BLUE printf("\e[1;34m");	 //蓝色字体

int maxfile = 0;                 //目录下的最大文件数
int restline = maxline;		 //一行的剩余长度,输出对齐	


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
void show(char *name);

void erro(char *str, int line)                     //错误处理函数
{          
	fprintf(stderr, "line: %d", line);
	perror(str);
   	exit(0);
}          

void show(char *name)
{
	int i, len;
	struct stat buf;
	if(lstat(name, &buf) == -1)
		erro("lstat", __LINE__);
	if(restline < maxfile)
	{
		printf("\n");
		restline = maxline;
	}
	len = strlen(name);
	if(len < maxfile)
		len = maxfile - len;
	if(S_ISDIR(buf.st_mode))
	{
		BLUE
		printf("%s", name);
		CLOSE
	}
	else if(buf.st_mode & S_IXUSR)
	{
		GREEN
		printf("%s", name);
		CLOSE
	}
	else
		printf("%s", name);
	for(i = 0; i < len; i++)
		printf(" ");
	printf("  ");
	restline -= (maxfile + 2);
}

int my_readir(char * path, int flag)
{
	int i, j;
	int count = 0;
	DIR * dir;
	struct dirent * ptr;
	char name[50000][50] = {0};
	struct stat buf;                 
	if(lstat(path, &buf) == -1)
		erro("lstat", __LINE__);
	if(S_ISDIR(buf.st_mode))
	{	
		if(chdir(path) < 0)
			erro("chdir", __LINE__);
		char tempp[500];
			getcwd(tempp, 500);
		if((dir = opendir(tempp)) == NULL)
			erro("opendir", __LINE__);
		while((ptr = readdir(dir)) != NULL)
		{
			if(maxfile < strlen(ptr->d_name))
			{
				maxfile = strlen(ptr->d_name);
			}
			strcpy(name[count], ptr->d_name);
			count++;
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
			case 0:										//ls 
				for(i = 0, j = 0; i < count; i++)
				{
					if(name[array[i]][0] == '.')       //跳过隐藏文件
						continue;
					show(name[array[i]]);
				}
				printf("\n");
				break;
			case 1:                                     // ls -a     
				for(i = 0; i < count; i++)
				{
					show(name[array[i]]);
				}
				printf("\n");
				break;
			case 2:                                   //ls -l
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


void r(char *path, int flag) //递归
{
	char pathname[1000] = {0};
	struct stat buf;
	DIR *dir;
	struct dirent *ptr;
	char temp[1000];
	if(lstat(path, &buf) == -1)
		erro("lstat", __LINE__);
		if(chdir(path) == -1)
			erro("chdir", __LINE__);
		getcwd(temp, 1000);
		if((dir = opendir(temp)) == NULL)
			erro("opendir", __LINE__);
		while((ptr = readdir(dir)) != NULL)
		{
			if(lstat(ptr->d_name, &buf) == -1)
				erro("stat", __LINE__);
			if(S_ISDIR(buf.st_mode))
			{
				if (strcmp(".", ptr->d_name) == 0|| strcmp("..", ptr->d_name) == 0)
                	continue;
				else
				{
					strcpy(pathname, temp);
					if(pathname[strlen(pathname) - 1] != '/')
						pathname[strlen(pathname)] = '/';
					strcat(pathname, ptr->d_name);
					YELLOW
					printf("%s\n", pathname);
					CLOSE
					my_readir(pathname, flag - 4);
					r(pathname, flag);
					memset(pathname, '\0', 1000);
				}
			}
		}
		chdir("..");                                                  //访问完当前目录后返回上层目录
		closedir(dir);
}

void print(char * name)
{       
        int mode;
        char buf_time[32];
		struct stat buf;
        struct group *grp;    				  //获取文件所属用户组名
        struct passwd *pwd;   				  //获取文件所属用户名
        if(lstat(name, &buf) == -1)
			erro("lstat", __LINE__);
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
                         
       
        printf(" %ld", buf.st_nlink); 			     //打印文件链接数
        pwd = getpwuid(buf.st_uid);
        grp = getgrgid(buf.st_gid);
        printf(" %s", pwd->pw_name);
        printf(" %s", grp->gr_name);
       
        printf(" %ld", buf.st_size); 				 //打印文件大小
        strcpy(buf_time, ctime(&buf.st_mtime));      //取出文件的时间
        buf_time[strlen(buf_time) - 1]  = '\0';      //去掉换行符
        printf(" %s", buf_time);				     //打印文件时间信息
		/*根据权限打印文件名*/
		if(S_ISDIR(mode))
		{
			BLUE
			printf(" %s\n", name);
			CLOSE
		}
		else if(mode & S_IXUSR)
		{
			GREEN
			printf(" %s\n", name);
			CLOSE
		}
		else
			printf(" %s\n", name);
}


int main(int argc, char * argv[])
{
	//printf("argc: %d\n", argc);
	int flag = 0;
	int i, j;
	int len;
	char pathname[100] = {0};
	char path[100] = {0};
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
	if(argc < 1)
		erro("输入错误", __LINE__);
	else if(argc == 1)
	{
		if(getcwd(pathname, 100) == NULL)
			erro("getcwd", __LINE__);
		my_readir(pathname, flag);
	}
	else if(argc == 2)
	{
		if(flag == 0)
		{
			strcpy(pathname, argv[1]);
			my_readir(pathname, flag);
		}
		else if(flag >= 4)
		{
			if(getcwd(pathname, 100) == NULL)
				erro("getcwd", __LINE__);
			printf("目录路径:%s\n", pathname);
			my_readir(pathname, flag - 4);
			r(pathname, flag);
		}
		else
		{
			if(getcwd(pathname, 100) == NULL)
				erro("getcwd", __LINE__);
			my_readir(pathname, flag);
		}
	}
	if(argc > 2)
	{
		if(flag == 0)
			erro("flag", __LINE__);
		else if(flag < 4)
		{
			strcpy(pathname, argv[argc - 1]);
			my_readir(pathname, flag);
		}
		else if(flag >= 4)
		{
			strcpy(pathname, argv[argc -1]);
			my_readir(pathname, flag - 4);
			r(pathname, flag);
		}
	}
	return 0;
}

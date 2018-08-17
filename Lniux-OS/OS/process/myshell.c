#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<dirent.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

//int sigemptyset(sigset_t *set);

#define normale 0       							                 //一般命令
#define out_redirect 1  						                     //输出重定向
#define in_redirect 2                                                //输入重定向
#define have_pipe 3                                                  //命令中有管道
#define out 4                                                        //追加输入重定向
#define in 5                                                         //追加输出重定向
#define CD 6														//切换工作目录

#define GREEN printf("\e[1;33m");                                    //绿色字体 
#define CLOSE printf("\033[0m");                                     //关闭彩色字体
#define BLUE printf("\e[1;34m");                                     //蓝色字体

void print_prompt();                                                 //打印终端名称
void get_input(char *buf); 				                             //获得一条用户输入的执行命令
void explain_input(char *buf, int *argcount, char arglist[100][256]); //解析命令
void do_cmd(int argcount, char arglist[100][256], char lastpath[]);   //执行命令
int  find_command(char *command);                                     //查找是否有该命令的执行文件

int main(int argc, char *argv)
{
	int i;
	int argcount = 0;
	char arglist[100][256];                                         
	char **arg = NULL;                                                   
	char *buf = malloc(sizeof(256));                                               //获取终端输入字符
	char lastpath[1000] = {0};                                      //保留上一次切换目录时的名称 用于cd -
	sigset_t intmask;
	sigemptyset(&intmask);                                          //将信号集设置为空
	sigaddset(&intmask, SIGINT);                                    //加入中断ctrl+c信号
	sigprocmask(SIG_BLOCK,&intmask,NULL);
	print_prompt();
	while(1)
	{
		/*将buf所指向的空间凋零*/
		//memset(buf, '\0', 256);
		//get_input(buf);
		/*若输入的命令为exit或logout则退出本程序*/
		buf = readline(NULL);
		add_history(buf);
		buf[strlen(buf)] = '\n';
		if(strcmp(buf, "exit\n") == 0 || strcmp(buf, "logout\n") == 0)
			break;
		for(i = 0; i < 100; i++)
		{
			arglist[i][0] = '\0';
		}
		argcount = 0;
		explain_input(buf, &argcount, arglist);
		do_cmd(argcount, arglist, lastpath);
		print_prompt();
	}
	if(buf != NULL)
	{
		free(buf);
		buf = NULL;
	}
	exit(0);
	return 0;
}

void print_prompt()
{
	char path[1000] = {0};
	char *ptr;
	char temp[11] = {0};
	int i;
	getcwd(path, 1000);
	for(i = 0; i < 11; i++)
		temp[i] = path[i];
	GREEN
	printf("xuexi@xuexi1");
	CLOSE
	printf(":");
	if(strncmp(temp, "/home/xuexi", 11) == 0)
	{
		BLUE
		printf("~");
		ptr = &path[11];
		printf("%s", ptr);
		CLOSE
	}
	else
	{
		BLUE
		printf("%s", path);
		CLOSE
	}
	printf("$ ");
}

/*输入命令并将其保存在buf中*/
void get_input(char *buf)
{
	int len = 0;
	int ch;
	ch = getchar();
	while(ch != '\n' && len < 256)
	{
		buf[len++] = ch;
		ch = getchar();
	}
	if(len == 256)
	{
		perror("command is too long");
		exit(0);
	}
	buf[len++] = '\n';
	buf[len] = '\0';
}

/*解析buf中的命令，将结果存入arglist中，命令以回车符号\n结束*/
void explain_input(char *buf, int *argcount, char arglist[100][256])
{
	char *p = buf;
	char *q = buf;
	int number = 0;
	
	while(1)
	{
		if(p[0] == '\n')        //将命令参数解析完，退出循环
			break;
		if(p[0] == ' ')
		{
			p++;
		}
		else
		{
			q = p;
			number = 0;
			while(q[0] != ' ' && q[0] != '\n')
			{
				number++;
				q++;
			}
			strncpy(arglist[*argcount], p, number + 1); //将命令存入arglist中
			arglist[*argcount][number] = '\0';
			*argcount = *argcount + 1;
			p = q;
		}	
	}
}

void do_cmd(int argcount, char arglist[100][256], char lastpath[])
{
	int flag = 0;
	int how = 0;               //用于指示命令中是否含有>, < |
	int background = 0;		   //表示命令中是否以后台运行表示符&
	int status;
	int i;
	int fd;
	char *arg[argcount + 1];
	char *argnext[argcount + 1];
	char *file;
	char *path;
	pid_t pid;
	/*将命令取出*/
	for(i = 0; i < argcount; i++)
	{
		arg[i] = (char *)arglist[i];
	}
	arg[argcount] = NULL;
	for(i = 0; i < argcount; i++)               //判断是否有后台运行符
	{
		if(strncmp(arg[i], "&", 1) == 0)
		{
			if(i == argcount - 1)
			{
				background = 1;
				arg[argcount - 1] = NULL;
				break;
			}
			else
			{
				perror("wrong command ///");
				return;
			}
		}
	}
	for(i = 0; arg[i] != NULL; i++)
	{
		if(strcmp(arg[i], ">") == 0)
		{
			flag++;
			how = out_redirect;
			if(arg[i + 1] == NULL)
				flag++;
		}
		if(strcmp(arg[i], "<") == 0)
		{
			flag++;
			how = in_redirect;
			if(i = 0)
				flag ++;
		}
		if(strcmp(arg[i], "|") == 0)
		{
			flag++;
			how = have_pipe;
			if(arg[i + 1] == NULL)
				flag++;
			if(i == 0)
				flag++;
		}
		if(strcmp(arg[i], "cd") == 0)
		{
			flag++;
			how = CD;
		}
	}
	/*如果flag大于1，说明命令中有多个>, <, |符号，本程序是不支持这样的命令的，或者命令格式不对*/
	if(flag > 1)
	{
		perror("wrong command .. ......");
		return;
	}
	if(how == out_redirect)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], ">") == 0)
			{
				file = arg[i+1];
				printf("230 file %s\n", file);
				arg[i] = NULL;
			}
		}
	}
	if(how == in_redirect)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], "<") == 0)
			{
				file = arg[i + 1];
				printf("226 file %s", file);
				arg[i] = NULL;
			}
		}
	}
	if(how == have_pipe)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], "|") == 0)
			{
				arg[i] = NULL;
				int j;
				for(j = i + 1; arg[j] != NULL; j++)
				{
					argnext[j - i - 1] = arg[j];
				}
				argnext[j - i -1] = arg[j];
				break;
			}
		}
	}
	if(how == CD)            //获取需要切换的目录
	{
		if(arg[1] == NULL)
			path = "/home/xuexi";
		else
		{
			path = arg[1];
		}
	}
	if((pid = fork()) < -1)
	{
		perror("fork erro");
		return;
	}
	switch(how)
	{
		case 0:
			if(pid == 0)
			{
				if( !(find_command(arg[0])) )
				{
					printf("%s :command not found\n", arg[0]);
					exit(0);
				}
				execvp(arg[0], arg);
				exit(0);
			}
			break;
		case 1:
			if(pid == 0)
			{
				if(!(find_command(arg[0])) )
				{
					printf("%s :command not found\n", arg[0]);
					exit(0);
				}
				fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
				dup2(fd, 1);
				execvp(arg[0], arg);
				exit(0);
			}
			break;
		case 2:
			if(pid == 0)
			{
				if(!(find_command(arg[0])) )
				{
					printf("%s :command not found\n", arg[0]);
					exit(0);
				}
				fd = open(file, O_RDONLY);
				dup2(fd, 0);
				execvp(arg[0], arg);
				exit(0);
			}
			break;
		case 3:
			if(pid == 0)
			{
				int pid2;
				int status2;
				int fd2;
				if( (pid2 = fork()) < 0)
				{
					printf("fork2 erro\n");
					return;
				}
				else if(pid2 == 0)
				{
					if( !(find_command(arg[0])) )
					{
						printf("%s : command not found\n", arg[0]);
						exit(0);
					}
					fd2 = open("/tmp/youdonotknowfile", O_WRONLY | O_CREAT |O_TRUNC, 0644);
					dup2(fd2, 1);
					execvp(arg[0], arg);
					exit(0);
				}
				if(waitpid(pid2, &status2, 0) == -1)
					printf("wait for child process error\n");
				if(! (find_command(argnext[0])) )
				{
					printf("%s :command not found\n", argnext[0]);
					exit(0);
				}
				fd2 = open("/tmp/youdonotknowfile", O_RDONLY);
				dup2(fd2, 0);
				execvp(argnext[0], argnext);
				if( remove("/tmp/youdonotknowfile") )
						printf("remove erro\n");
				exit(0);
			}
			break;
		case 6:
			if(pid != 0)
			{
				if(path[0] == '-')
				{
					chdir(lastpath);
				}
				else
				{
					getcwd(lastpath, 1000);
					chdir(path);
				}
				break;
			}
			exit(0);
		default:
			break;
	}
	/*若命令中有&,表示后台执行，父进程直接返回，不等待子进程结束*/
	if(background == 1)
	{
		printf("process id %d\n", pid);
		return;
	}
	/*父进程等待子进程结束*/
	if(waitpid(pid, &status, 0) == -1)
		printf("wait for child process error\n");
}
	
int find_command(char *command)
{
	DIR *dir;
	struct dirent *ptr;
	char *path[] = {"./", "/usr/local/bin", "/bin", "/usr/bin", NULL};

	/*使当前目录下的程序可以运行*/
	if(strncmp(command, "./", 2) == 0)
		command += 2;
	/*分别在当前目录，/bin和/usr/bin目录查找要执行的程序*/
	int i = 0;
	while(path[i] != NULL)
	{
		if((dir = opendir(path[i])) == NULL)
			printf("can not open /bin \n");
		while( (ptr = readdir(dir)) != NULL)
		{
			if(strcmp(ptr->d_name, command) == 0)
			{
				closedir(dir);
				return 1;
			}
		}
		closedir(dir);
		i++;
	}
	return 0;
}

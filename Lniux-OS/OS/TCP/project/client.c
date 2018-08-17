#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
/*客户端*/
#define PORT 8848           //端口地址
#define send_length 200     
#define CLOSE printf("\033[0m");     //关闭彩色字体                         
#define GREEN printf("\e[1;32m");      //绿色字体
#define RED printf("\e[1;31m");        //红色字体
#define CLEAR printf("\033[2J");       //清屏
#define YELLOW printf("\e[1;33m");     //黄色字体
#define BLUE printf("\e[1;34m");       //蓝色字体

char *IP = "128.0.0.208";
/*错误处理函数*/
void my_err(char *str, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(str);
	exit(0);
}
/*注册*/
int user_login(int socket)	
{
	char use[200] = {0};
	char username[21] = {0};
	char passwd[21] = {0};
	char ans[21] = {0};
	char flag[200] = {0};
	int recv_length = 200;
	printf("----------------------\n");
	printf("\t\t注册\n");
	printf("用户名长度小于20\n");
	printf("请输入要注册的用户名：");
	getchar();
	scanf("%[^\n]", username);
	username[strlen(username)] = ':';     //分隔名字，密码，密保问题的分隔符
	username[strlen(username)] = '\0';

	printf("请输入密码，长度小于20: ");
	getchar();
	scanf("%[^\n]", passwd);
	passwd[strlen(passwd)] = ':';
	passwd[strlen(passwd)] = '\0';

	printf("密保问题: 你帅不帅\n");
	getchar();
	scanf("%[^\n]", ans);
	ans[strlen(ans)] = '\0';
	strcpy(use, "login:");
	strcat(use, username);
	strcat(use, passwd);
	strcat(use, ans);
	if(send(socket, use, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	while( (len = recv(socket, flag, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(flag, "success", 7) == 0)
	{
		printf("注册成功\n");
		printf("----------------------\n");
		return 1;
	}
	else
		return 0;
}
/*登录*/
int user_enter(int socket)
{
	char use[200] = "enter:";
	char name[21] = {0};
	char passwd[20] = {0};
	char flag[200] = {0};
	int recv_length = 200;      
	printf("------------------\n");
	printf("登录界面\n");
	printf("请输入你的用户名：");
	getchar();
	scanf("%[^\n]", name);
	name[strlen(name)] = ':';
	name[strlen(name)] = '\0';
	printf("请输入你的密码：");
	getchar();
	scanf("%[^\n]", passwd);
	passwd[strlen(passwd)] = '\0';
	strcat(use, name);
	strcat(use, passwd);
	if(send(socket, use, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	while( (len = recv(socket, flag, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(flag, "success", 7) == 0)
	{
		printf("登录成功\n");
		printf("------------------\n");
		return 1;
	}
	else
		return 0;
}
//找回密码
int find_passwd(int socket)
{
	char answer[200] = "find_passwd:";
	char name[21] = {0};								//用户名
	char ans[21] = {0};									//回答
	char passwd[200] = {0};                    		    //找回的密码
	printf("-------------------\n");
	printf("找回密码\n");
	printf("请输入你的用户名\n");
	getchar();
	scanf("%[^\n]", name);
	name[strlen(name)] = ':';
	name[strlen(name)] = '\0';
	printf("问题：你帅不帅\n");
	getchar();
	scanf("%[^\n]", ans);
	/*发送请求及用户名和答案*/
	strcat(answer, name);           
	strcat(answer, ans);		
	if(send(socket, answer, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	int recv_length = 200;
	while( (len = recv(socket, passwd, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(passwd, "fault", 5))
	{
		//strcpy(passwd, flag);
		printf("回答正确，您的密码是：%s\n", passwd);
		printf("---------------\n");
		return 1;
	}
	else
		return 0;
}
/*是否添加好友*/
void add_friend(char *str, int socket)
{
	char ch;
	char flag[200] = {0};
	int count = 0, j = 0, k = 0;
	char name[20] = {0};
	char message[200] = {0};
	for(int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
			k = i;
		}
		if(count == 1)
		{
			if(str[i] ==' ')
				break;	
			name[j++] = str[i];
		}
	}
	strcpy(message, &str[k]);
	printf("好友添加请求 %s\n", message);
	printf("是否同意，同意请输入@agree:加对方姓名， 不同意请输入@disagree:加对方姓名\n");
}

/*查看好友状态信息*/
void watch_friend(char *str)
{
	int i, j = 0, count = 0;
	int len = 0;
	char name[200] = {0};
	char message[200] = {0};
	int flag;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
		}
		if(str[i] == ' ')
		{
			count++;
			i++;
		}
		if(count == 1)
		{
			name[j++] = str[i];               //获取用户名
		}
		if(count == 2)
		{
			len = strlen(name);
			for(j = 0; j < 30 - len; j++)
				message[j] = ' ';
			if(strcmp(&str[i], "online") == 0)
				flag = 0;
			else
				flag = 1;
			strcat(message, &str[i]);
			break;
		}
	}
	if(flag == 0)
	{
		printf("\t%s", name);
		GREEN
		printf("%s\n", message);
		CLOSE
	}
	else
	{
		printf("\t%s", name);
		RED
		printf("%s\n", message);
		CLOSE
	}
}

/*接收离线消息*/
void recv_record(char *str)
{
	int i, j = 0, count = 0;
	char message[200] = {0};
	for(i = 0; str[i] != 0; i++)
	{
		if(str[i] == ':')
			count++, i++;
		if(count == 1)
			message[j++] = str[i];
	}
	YELLOW
	printf("您有离线消息: %s\n", message);
	CLOSE
}

void *send_message(void *arg)
{
	int socket = *(int *)arg;
	char message[200] = {0};
	char message1[200] = {0};
	while(1)
	{
		message[0] = '~';
		getchar();
		scanf("%[^\n]", message1);
		strcat(message, message1);
		if(send(socket, message, send_length, 0) < 0)
			my_err("send", __LINE__);
		memset(message, 0, sizeof(message));
		memset(message1, 0, sizeof(message1));

	}
}

void *recv_message(void *arg)
{
	int socket = *(int *)arg;
	char readbuf[200] = {0};
	int len;
	while(1)
	{	
		int len, count = 0;
		int recv_length = 200;
		while( (len = recv(socket, readbuf, recv_length, 0)) )
		{
			count += len;
			if(count == 200)
				break;
			else
				recv_length -= len;
		}
		readbuf[strlen(readbuf)] = '\0';
		if(len == -1)
			my_err("recv", __LINE__);
		readbuf[strlen(readbuf)] = '\0';
		if(strncmp(readbuf, "add", 3) == 0)
		{
			add_friend(readbuf, socket);
		}
		else if(strncmp(readbuf, "#", 1) == 0)          //通知
		{
			printf("----------------------------------------------------\n");
			BLUE
			printf("通知: %s\n", &readbuf[1]);
			CLOSE
			printf("----------------------------------------------------\n");
		}
		else if(strncmp(readbuf, "@friend:", 8) == 0)
			watch_friend(readbuf);
		else if(strncmp(readbuf, "off:", 4) == 0)
			recv_record(readbuf);
		else
			printf("\t\t\t%s\n", readbuf);
		memset(readbuf, 0, sizeof(readbuf));
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
		my_err("input erro", __LINE__);
	int cli_fd;
	int status;
	char message[1024] = {0};
	char message1[1024] = {0};
	struct sockaddr_in serv_addr;
	socklen_t len;
	int select, flag;
	if( (cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("socket", __LINE__);

	/*设置连接的结构*/
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	len = sizeof(struct sockaddr_in);
	if(connect(cli_fd, (struct sockaddr *)&serv_addr, len) < 0)
		my_err("connect", __LINE__);
	printf("请输入1(注册)，2(登录)或者3(找回密码)\n");
	scanf("%d", &select);
	switch(select)
	{
		case 1:
			while(!user_login(cli_fd))
				printf("注册失败，请重新注册\n");
			while(!user_enter(cli_fd))	
				printf("登录失败请重新登录\n");
			break;
		case 2:
			while(!user_enter(cli_fd))
				printf("登录失败，请重新登录\n");
			break;
		case 3:
			while(!find_passwd(cli_fd))
				printf("查找失败，请重新查找\n");
			while(!user_enter(cli_fd))	
				printf("登录失败请重新登录\n");
			break;
	}
	printf("添加好友在屏幕上输入     ");
	RED
	printf("add:加好友名字\n");
	CLOSE
	printf("删除好友在屏幕上输入     ");
	RED
	printf("rm:加好友名字\n");
	CLOSE
	printf("发送消息输入             ");
	RED
	printf("用户名:加消息\n");
	CLOSE
	printf("查看好友列表             ");
	RED
	printf("按回车即可\n");
	CLOSE
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, send_message, (void *)&cli_fd);
	pthread_create(&tid2, NULL, recv_message, (void *)&cli_fd);
	pthread_join(tid1, (void *)&status);
	pthread_join(tid2, (void *)&status);
}


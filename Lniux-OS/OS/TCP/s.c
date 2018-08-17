#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

void my_err(char *string, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(string);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, newfd;
	struct sockaddr_in addr, cli_addr;          //服务端的地址和客户端的地址
	char buf[1024];
	pid_t pid;
	int portnumber;
	int status, ret;
	socklen_t len;
	if(argc != 2)
		my_err("input erro", __LINE__);
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("socket", __LINE__);
	//初始化服务器端口地址
	portnumber = atoi(argv[1]);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;                     //ipv4
	addr.sin_port = htons(portnumber);				   //端口赋值
	addr.sin_addr.s_addr = htonl(INADDR_ANY);      //任意地址

	//将套接字连接到本地端口
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
		my_err("bind", __LINE__);
	if(listen(sockfd, 10) < 0)
		my_err("listen", __LINE__);
	while(1)
	{
		//通过accept接收客户端的连接请求，并返回连接套接字用于收发数据
		len = sizeof(struct sockaddr);
		if( (newfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len)) < 0)
			my_err("accept", __LINE__);
		printf("accept a new client, ip:%s\n", inet_ntoa(cli_addr.sin_addr));
		pid = fork();
		if(pid == 0)
		{
			//while(1)
			//{
				if((ret = send(newfd, "hello client", 13, 0)) < 0)
					my_err("send", __LINE__);
			//}
			close(newfd);
			close(sockfd);
			exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			exit(0);
		}
		
	}
	return 0;
}

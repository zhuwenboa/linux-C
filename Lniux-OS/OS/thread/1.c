#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void *arg)
{
	char *name = (char *)arg;
	char *str = "hi";
	printf("hello everyone\n");
	printf("%s\n", name);
	name = str;

}

int main()
{
	char *name = "hello,word!";
	pthread_t tid1;
	pthread_create(&tid1, NULL, (void *)thread, (void *)name);
	pthread_join(tid1, NULL);
	printf("%s\n", name);
	printf("啊哈\n");
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int count = 0;

void *thread1(void)
{
	count++;
	printf("thread1 count : %d\n", count);
}

void *thread2(void)
{
	count++;
	printf("thread2 count : %d\n", count);
}

void *thread3(void)
{
	count++;
	printf("thread3 count : %d\n", count);
}

int main(void)
{
	pthread_t tid1, tid2, tid3;
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	pthread_create(&tid3, NULL, (void *)thread3, NULL);
	sleep(3);
	printf("count is %d\n", count);
	return 0;
}

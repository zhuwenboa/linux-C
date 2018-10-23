#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

/*               函数作用及概念
pthread_cleanup_push(pthread_mutex_unlock, (void *) &mut);
pthread_mutex_lock(&mut);
 //do some work 
pthread_mutex_unlock(&mut);
pthread_cleanup_pop(0);
本来do some work之后是有pthread_mutex_unlock(&mut);这句,也就是有解锁操作,但是在do some work时会出现非正常终止,那样的话,系统会根据pthread_cleanup_push中提供的函数,和参数进行解锁操作或者其他操作,以免造成死锁!
*/



pthread_mutex_t mutex;       //初始化一个互斥锁
pthread_cond_t cond;		//初始化一个条件变量

void *thread1(void *arg)
{
	//pthread_cleanup_push(pthread_mutex_unlock, &mutex);
	while(1)
	{
		printf("thread1 is running\n");
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("threa1 applied the condition\n");
		pthread_mutex_unlock(&mutex);
		//sleep(4);
	}
	//pthread_cleanup_pop(0);
}

void *thread2(void *arg)
{
	while(1)
	{
		printf("thread2 is running\n");
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("thread2 applied the condition\n");
		pthread_mutex_unlock(&mutex);
		//sleep(1);
	}
}

int main(void)
{
	pthread_t tid1, tid2;
	int n = 10;
	printf("condition variable study\n");
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	
	do
	{
		pthread_cond_signal(&cond);
	}while(1);
	sleep(10);
	pthread_exit(0);
}

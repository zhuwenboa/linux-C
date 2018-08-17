#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond1, cond2, cond;

void *thread1(void)
{
	printf("thread1 is runnig\n");
	pthread_mutex_lock(&mutex);
	printf("thread1 mutex is locked\n");
    pthread_cond_wait(&cond, &mutex);
	printf("thread1 cond1 is applied\n");
	pthread_mutex_unlock(&mutex);
	//sleep();
}

void *thread2(void)
{
	printf("thread2 is running\n");
	pthread_mutex_lock(&mutex);
	printf("thread2 mutex is locked\n");
	pthread_cond_wait(&cond, &mutex);
	printf("thread2 cond2 is applied\n");
	pthread_mutex_unlock(&mutex);
	printf("thread2 mutex unlock\n");
}

int main(void)
{

	pthread_t tid1, tid2;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	//pthread_cond_init(&cond, NULL);
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	while(1)
	{
		pthread_cond_broadcast(&cond);
	}
	sleep(5);
	printf("=========\n");
	return 0;
}

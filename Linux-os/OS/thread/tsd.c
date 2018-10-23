#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

pthread_key_t key;

void *thread2(void *arg)
{
	int tsd = 5;
	printf("thread2 %ld is running\n", pthread_self());
	pthread_setspecific(key, &tsd);
	printf("thread2 %ld returns %p\n", pthread_self(), pthread_getspecific(key));
}

void *thread1(void *arg)
{
	int tsd = 0;
	pthread_t thid2;
	printf("thread1 %ld is running\n", pthread_self());
	pthread_setspecific(key,  &tsd);
	printf("thread1 %ld returns %p\n", pthread_self(), pthread_getspecific(key));
	pthread_create(&thid2, NULL, thread2, NULL);
	sleep(5);
//	printf("thread1 %ld returns %p\n", pthread_self(), pthread_getspecific(key));
}

int main()
{
	pthread_t thid1;
	printf("main thread begins running \n");
	pthread_key_create(&key, NULL);
	pthread_create(&thid1, NULL, (void *)thread1, NULL);
	sleep(3);
	pthread_key_delete(key);
	printf("main thread exit\n");
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int count = 3;
pthread_key_t key;
void *thread1(void)
{
	printf("thread1 is running\n");
	pthread_setspecific(key, (void *)count);
	printf("count: %d\n", ++count);
	//pthread_key_delete(key);
}

void *thread2(void)
{
	pthread_t tid1;
	printf("thread2 is running\n");
	pthread_setspecific(key, (void*)count);
//	printf("count2: %d\n", count);
	//pthread_key_delete(key);
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	printf("count2: %d\n", count);
}
int main()
{
	pthread_t tid2;
	pthread_key_create(&key, NULL);
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	sleep(3);
	pthread_key_delete(&key);
	printf("main thread exit\n");
	return 0;
}

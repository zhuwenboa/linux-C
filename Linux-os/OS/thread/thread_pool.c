#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<assert.h>
#include<sys/wait.h>

typedef struct worker
{
	void*(*process)(void * arg);
	void *arg;
	struct worker *next;
}create_worker;
/*线程池结构*/
typedef struct
{
	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_cond;
	/*链表结构,线程池中有所等待任务*/
	create_worker *queue_head;
	/*是否销毁线程池*/
	int shutdown;
	pthread_t *threadid;
	/*线程池中允许的活动线程数目*/
	int max_thread_num;
	/*当前等待队列的任务数目*/
	int cur_queue_size;
}create_pool;
void pool_add_worker(void*(*process)(void *arg), void *arg);
void *thread_routine(void *arg);

static create_pool *pool = NULL;

/*创建线程池*/
void pool_init(int max_thread_num)
{
	pool = (create_pool *)malloc(sizeof(create_pool));
	pthread_mutex_init(&(pool->queue_mutex), NULL);
	pthread_cond_init(&(pool->queue_cond), NULL);
	pool->queue_head = NULL;
	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;
	pool->shutdown = 0;
	pool->threadid = (pthread_t *)malloc(sizeof(pthread_t) * max_thread_num);
	for(int i = 0; i < max_thread_num; i++)
	{
		pthread_create(&pool->threadid[i], NULL, thread_routine, NULL);
	}
}

/*向线程池中加入任务*/
void pool_add_worker(void*(*process)(void *arg), void *arg)
{
	/*构建一个新任务*/
	create_worker *newworker = (create_worker*)malloc(sizeof(create_worker));
	newworker->process = process;
	newworker->arg = arg;
	newworker->next = NULL;
	pthread_mutex_lock(&pool->queue_mutex);
	/*将任务加入到等待队列*/
	create_worker *member = pool->queue_head;
	if(member != NULL)
	{
		while(member->next != NULL)
			member = member->next;
		member->next = newworker;
	}
	else
		pool->queue_head = newworker;
	assert(pool->queue_head != NULL);
	pool->cur_queue_size++;
	pthread_mutex_unlock(&pool->queue_mutex);
	/*等待队列有任务，唤醒一个等待的线程*/
	pthread_cond_signal(&pool->queue_cond);
}

/*销毁线程池， 等待队列中的任务不会在执行，但是正在运行的线程一定会把任务运行完后在退出*/
int pool_destroy()
{
	if(pool->shutdown)
		return -1;		//防止两次调用	
	pool->shutdown = 1;
	/*唤醒所有的等待线程，线程池要销毁了*/
	pthread_cond_broadcast(&pool->queue_cond);

	/*阻塞等待线程退出，否则就成僵尸了*/
	for(int i = 0; i < pool->max_thread_num; i++)
		pthread_join(pool->threadid[i], NULL);      //等待所有线程执行完毕再销毁
	free(pool->threadid);
	/*销毁等待队列*/
	create_worker *head = NULL;
	while(pool->queue_head != NULL)
	{
		head = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		free(head);
	}

	/*销毁条件变量和互斥量*/
	pthread_mutex_destroy(&pool->queue_mutex);
	pthread_cond_destroy(&pool->queue_cond);
	free(pool);
	pool = NULL;
}

void *thread_routine(void *arg)
{
	printf("线程开始运行 %ld\n", pthread_self());
	while(1)
	{
		pthread_mutex_lock(&pool->queue_mutex);
		/*如果等待队列为0并且不销毁线程池，则处于阻塞状态*/
		while(pool->cur_queue_size == 0 && !pool->shutdown)
		{
			printf("thread: %ld is wait\n", pthread_self());
			pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
		}
		printf("shutdown is: %d\n", pool->shutdown);
		/*线程池要销毁了*/
		if(pool->shutdown)
		{
			/*遇到break, continue, return 等跳转语句，要记得先解锁*/
			pthread_mutex_unlock(&pool->queue_mutex);
			printf("thread %ld will exit\n", pthread_self());
			pthread_exit(NULL);
		}
		printf("thread %ld is starting to work\n", pthread_self());
		
		assert(pool->cur_queue_size != 0);
		assert(pool->queue_head != NULL);
		/*等待队列长度减１，并去除链表中的头元素*/
		pool->cur_queue_size--;
		create_worker *work = pool->queue_head;
		pool->queue_head = work->next;
		pthread_mutex_unlock(&pool->queue_mutex);

		/*调用回调函数，执行任务*/
		(*(work->process)) (work->arg);
		free(work);
		work = NULL;
	}
	//这一句应该不可以到达执行
	pthread_exit(NULL);
}

void *myprocess(void *arg)
{
	printf("thread is running now thread id is :%ld and working on task %d\n", pthread_self(), *(int *)arg);
	sleep(1);
	return NULL;
}

int main(int argc, char *argv[])
{
	pool_init(3);    //创建线程池，池中三个活动的线程
	sleep(3);	
	/*连续向池中投入十个任务*/
	int *work_max_num = (int *)malloc(sizeof(int) * 10);
	int i;
	for(i = 0; i < 10; i++)
	{
		work_max_num[i] = i;
		pool_add_worker(myprocess, &work_max_num[i]);
	}
	/*等待所有任务完成*/
	sleep(5);
	/*销毁线程池*/
	pool_destroy();
	free(work_max_num);
	return 0;
}

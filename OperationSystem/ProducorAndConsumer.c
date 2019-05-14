#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define PRODUCE_SPEED 1 // 生产速度
#define CONSUM_SPEED  1 // 消费速度
#define INIT_NUM 6 // 仓库原有产品数量
#define TOTAL_NUM 10 // 仓库容量
#define mysleep(x) usleep(x*1000) // 在头文件<unistd.h>,暂时挂起，时间是微秒

sem_t p_sem, c_sem, sh_sem; // 生产者信号量，消费者信号量，共享信号量
int num = INIT_NUM;

void produce(void)
{
	sleep(CONSUM_SPEED);
}

void consum(void)
{
	sleep(PRODUCE_SPEED);
}

int add()
{
	num++;
	mysleep(500);
	return num;
}

int removed()
{
	num--;
	mysleep(500);
	return num;
}

void *productor(void *arg)
{
	while(1)
	{
		produce();
		/** 如果以下两个P操作调换顺序会发生死锁
		 * 如果调换之后，若消费者没有消费，生产者就去生产
		 * 但是如果仓库已满，则消费者等待消费者消费
		 * 此时生产者在仓库内，消费者又不能进入仓库消费
		 * 所以进入死锁
		 */
		sem_wait(&p_sem);
		sem_wait(&sh_sem);
		add(); // 生产者生产
		printf("%d is produced.\n",num);
		sem_post(&sh_sem);
		sem_post(&c_sem);
		produce();
	}
}

void *consumer(void *arg)
{
	while(1)
	{
		sem_wait(&c_sem);
		sem_wait(&sh_sem);
		printf("%d is consummed.\n",num);
		removed(); // 消费者消费
		sem_post(&sh_sem);
		sem_post(&p_sem);
		consum();
	}
}

int main()
{
	pthread_t tid1,tid2;
	sem_init(&p_sem, 0, TOTAL_NUM-INIT_NUM);
	sem_init(&c_sem, 0, INIT_NUM);
	sem_init(&sh_sem, 0, 1);

	pthread_create(&tid1,NULL,productor,NULL);
	pthread_create(&tid2,NULL,consumer,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	return 0;
}

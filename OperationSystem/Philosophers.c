#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>

#define mysleep(x) usleep(x*1000)
sem_t Fork[5]; // 哲学家左边筷子信号量
sem_t Room; // 同时拿起筷子的最大哲学家数

void think(int num)
{
	// 哲学家思考
	printf("philosopher:%d is thinking.\n",num);
	mysleep(2000);
}

void eating(int num)
{
	// 哲学家就餐
	printf("philosopher:%d is eating.\n",num);
	mysleep(2000);
}

void *philosopher(void *number)
{
	int num = (int)number;
	while(1)
	{
		think(num);
		sem_wait(&Room); // 哲学家准备进餐
		sem_wait(&Fork[num]); // 哲学家拿起左边筷子
		sem_wait(&Fork[(num+1)%5]); // 哲学家拿起右边筷子
		eating(num);
		sem_post(&Fork[(num+1)%5]); // 哲学家放下右边筷子
		sem_post(&Fork[num]); // 哲学家放下左边筷子
		sem_post(&Room); // 哲学家停止进餐
	}
}

int main()
{
	pthread_t thinker[5];
	sem_init(&Room,0,4);
	for(int it=0;it<5;it++)
	{
		sem_init(&Fork[it],0,1);
	}
	for(int it=0;it<5;it++)
	{
		pthread_create(&thinker[it],NULL,philosopher,(void *)it);
	}
	for(int it=0;it<5;it++)
	{
		pthread_join(thinker[it],NULL);
	}
	return 0;
}

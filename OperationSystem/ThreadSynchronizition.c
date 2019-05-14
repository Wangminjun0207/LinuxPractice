#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
char message[] = "Hello world ";
void *thread_function(void *arg)
{
	printf("thread_function is running, Argument was %s\n",(char *)arg);
	sleep(3);
	strcpy(message,"Bye!");
	pthread_exit("Thank you for CPU time\n");
}
int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_create(&a_thread,NULL,thread_function,(void *)message);
	if(res!=0)
	{
		perror("Thread creation failed!\n");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for thread to finish...\n");
	res = pthread_join(a_thread,&thread_result);// 主线程等待新线程返回
	if(res!=0)
	{
		perror("thread join failed!\n");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined, it return %s\n",(char *)thread_result);
	printf("Message is now %s\n",message);
	exit(EXIT_SUCCESS);
}

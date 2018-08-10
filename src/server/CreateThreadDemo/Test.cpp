#include "Test.h"

void *Test::PrintHello(void* threadid){
	//对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
   int tid = *(int*)threadid;
   cout << "Hello, 线程 ID " << tid << endl;
   pthread_exit(NULL);
}
void Test::test(){
    pthread_t pth[NUM_THREADS];
	int indexes[NUM_THREADS];// 用数组来保存i的值
	for(int i=0; i < NUM_THREADS; i++ )
	{
		indexes[i] = i; //先保存i的值
		void *p  = &indexes[i];
		cout << "main() : 创建线程, " << i << endl;
		pthread_create(&pth[i], NULL, PrintHello, p);
   }
}

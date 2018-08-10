#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
using namespace std;
#define NUM_THREADS   5

class Test
{
    public:
        void static *PrintHello(void* threadid);
        void test();
};
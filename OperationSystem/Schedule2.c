#include<stdio.h>
#include<sys/time.h>
#include<malloc.h>
#include<stdlib.h>

#define  NR_TASKS             64   //系统支持的进程个数
#define  TASK_RUNNING          0   //就绪态
#define  TASK_UNINTERRUPTIBLE  2   //不可中断的睡眠状态
#define  TASK_ZOMBIE           3   //僵死态

//进程表项
struct task_struct{
    long pid;        //进程号
    long state;      //进程运行状态
    long priority;   //优先数
    long counter;    //进程剩余时间片
    long start_time;  //进程开始时间
    long excute_time; //进程执行时间
};

struct task_struct init_task = {
    .pid = 0,
    .state = 0,
    .priority = 0,
    .counter = 0,
    .start_time = 0,
    .excute_time = 0
};

struct task_struct *current = &init_task;
unsigned long volatile jiffies = 0; //系统滴答数
struct task_struct* task[NR_TASKS] = {&init_task,}; //进程指针数组
#define  FIRST_TASK    task[0]
#define  LAST_TASK     task[NR_TASKS-1]

struct run_q {       //进程就绪队列
    struct task_struct *data;
    struct run_q *next;
};
struct run_q *head=NULL,*end=NULL,*head1=NULL,*end1=NULL,*head2=NULL,*end2=NULL,*r_temp;

#define  N_PROCESS             5   //进程个数
#define  MAX_RUNTIME         100   //最长运行时间
int process[N_PROCESS][2]={{0,3},{2,6},{4,4},{6,5},{8,2}};//进程初始值
int totalExcuteTime = 0;             //cpu总的运行时间
int runState[N_PROCESS][MAX_RUNTIME] = {0};  //进程运行状态的记录
void checkProcessCome();  //判断是否有进程到达，如果有则创建进程
void pause();             //0号进程的运行体
void schedule_s();        //FCFS调度程序
void schedule_f();        //RR调度程序
void switch_to(int pid);  //进程切换
void init();              //基于优先级调度器的初始化
void run(int pid);        //普通进程的运行体
void myfork(int pid);     //进程创建
void delete(int pid);     //进程清除

typedef void funtype(void);
funtype *schedule = NULL;

int main(int argc,char **argv)
{
    int i,j;
    int choice;
    while(1){
        printf("please choice the schedule measure:\n");
        printf("s :SRT\n");
        printf("f :Feedback\n");
        printf("q :quit\n");
        choice = getchar();
        if(choice == '\n')
            choice = getchar();
        switch(choice){
            case 's': schedule = schedule_s;break;
            case 'f': schedule = schedule_f;break;
            case 'q': return 0;
            default : {
                schedule = NULL;
                printf("please input the true symbol(p or f or r)!\n\n");
                continue;
            }
        }
        printf("task id   start  excute\n");
        for(i=0;i<N_PROCESS;i++){
            printf("task %2d: %6d %6d\n",i+1,process[i][0],process[i][1]);
            totalExcuteTime+=process[i][1];
        }
        init();

        //打印进程调度情况
        printf("time   : 0%*c%d\n",totalExcuteTime-2,' ',totalExcuteTime);
        for(i=0;i<N_PROCESS;i++){
            printf("task %2d: ",i+1);
            for(j=0;j<totalExcuteTime;j++){
                if(runState[i][j]==1) printf("#");
                else printf(" ");
                runState[i][j] = 0;
            }
            printf("\n");
        }
        while((head!=NULL)&&(head!=end)){
            r_temp = head;
            head = head->next;
            free(r_temp);
        }
        if(head){
            free(head);
            head = NULL;
            end = NULL;
        }
        current = &init_task;
        jiffies = 0;
        totalExcuteTime = 0;
        printf("\n");
    }
    return 0;
}
void schedule_s(){
    int i,next,c,j,t;
    struct task_struct **p,**min;
    c = 9999;
    next = 0;
    j=0;
    i = NR_TASKS;
    p = &task[NR_TASKS];
    while(--i){
        if(!*--p)
            continue;
        if((*p)->state == TASK_RUNNING && (*p)->start_time < c)
        {
            if(j==0)
            {
                t=i;
                min=p;
                j=1;
            }
            if((*min)->counter>(*p)->counter)
            {
                min=p;
                t=i;
                c = (*p)->start_time,next = t;

            }
            if((*min)->counter==(*p)->counter)
            {
                c = (*p)->start_time,next = i;
            }
            else
            {
                c = (*p)->start_time,next = t;
            }
        }
    }
    switch_to(next);
}

void schedule_f()
{
    int next;
    next = 0;
    if(current->state != TASK_RUNNING)//process end
    {
        r_temp = head;
        if(head==end){
            head = NULL;
            end = NULL;
        }else{
            head = head->next;
            end->next = head;
        free(r_temp);
        }
    }
	else if(head)
	{
        while(1)
        {
        	struct run_q *p,*q,*r;
                head=head->next;
                end=end->next;
                p=head;
                r=head;
                q=end;
                if(head->next->data->priority>r->data->priority)
                {
                    r=head->next;
                }
                if(head==p)
                {
                    next = r->data->pid;
                    break;
                }
        }
    }
    switch_to(next);
}

void switch_to(int pid){
    if(pid)
        run(pid);
    else
        pause();
}

void myfork(int pid){
    struct timeval now;
    struct run_q *p;
    task[pid] = (struct task_struct*)malloc(sizeof(struct task_struct));
    task[pid]->state = TASK_UNINTERRUPTIBLE;
    task[pid]->pid = pid;
    gettimeofday(&now,0);
    srand(now.tv_usec);
    task[pid]->priority = 2 + (int)(4.0f*rand()/(RAND_MAX+1.0f));
    task[pid]->counter = process[pid-1][1]-task[pid]->excute_time;
    task[pid]->start_time = jiffies;
    task[pid]->excute_time = 0;
    task[pid]->state = TASK_RUNNING;

    p = (struct run_q*)malloc(sizeof(struct run_q));
    p->data = task[pid];
    if(head==NULL){
        head = end = p;
        head->next = end;
        end->next = head;
    }else{
        end->next = p;
        end = p;
        end->next = head;
    }
}

void delete(int pid){
    free(task[pid]);
}

void checkProcessCome(){
    int i;
    for(i=0;i<N_PROCESS;i++){
    if(process[i][0]==jiffies)
        myfork(i+1);
    }
}

void init(){
    int i;
    for(i=1;i<NR_TASKS;i++){
        task[i] = NULL;
    }
    checkProcessCome();
    schedule();
}

void pause(){
    current = task[0];
    jiffies++;
    totalExcuteTime++;
    checkProcessCome();
    schedule();
}

void run(int pid){
    int i;
    current = task[pid];
    runState[pid-1][jiffies] = 1;
    jiffies++;
    task[pid]->counter--;
    task[pid]->priority--;
    task[pid]->excute_time++;
    //判断进程是否运行完，如果是则将进程杀死
    if(task[pid]->excute_time==process[pid-1][1]){
        task[pid]->state = TASK_ZOMBIE;
    }
    //判断所有进程是否都运行完，如果是则结束
    if(jiffies>=totalExcuteTime) return;
    checkProcessCome();
    schedule();
}

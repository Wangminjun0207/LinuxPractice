#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void func_A()
{
    int i;
    char c[512];
    FILE *in1,*in2,*in3,*in4;
    in1=fopen("/proc/meminfo","r");//打开meminfo文件
    in2=fopen("/proc/loadavg","r");//打开loadavg文件
    in3=fopen("/proc/interrupts","r");//打开interrupts文件
    in4=fopen("/proc/filesystems","r");//打开filesystem文件
    //获取meminfo文件的前5条信息
	printf("meminfo文件前5条信息：\n");
    for(i=0;i<5;i++)
	{
        fgets(c,512,in1);
    	printf("%s",c);
	}
	//显示loadavg文件的内容
	printf("loadavg文件信息：\n");
    fgets(c,512,in2);
    printf("%s",c);
    //获取interrupts文件的前10条信息
	printf("interrupts文件的前10行信息:\n");
    for(i=0;i<10;i++)
	{
        fgets(c,512,in3);
    	printf("%s",c);
	}
    //获取filesystems文件的10条信息
	printf("filesystems文件的前10行信息:\n");
    for(i=0;i<10;i++)
	{
        fgets(c,512,in4);
    	printf("%s",c);
	}
    fclose(in1);
    fclose(in2);
    fclose(in3);
    fclose(in4);
}

void main(int argc,char *argv[])
{
    char c1,c2;
    if(argc==1)
    {
        func_A();
        return;
    }
    if(argc>1)
    {
        sscanf(argv[1],"%c%c",&c1,&c2);
        if(c1!='-')
        {
            printf("usage: observer [-s] [-l int dur]\n");
            return;
        }
    }
}


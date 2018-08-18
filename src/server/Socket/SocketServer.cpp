#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
 
#define SERVER_PORT 8888 //端口号，定义为宏方便以后直接修改
#define BACKLOG     10   
 
int main()
{
	int SocketServer;
	int SocketClient;
	struct sockaddr_in SocketServerAddr;//服务器地址结构
	struct sockaddr_in SocketClientAddr;
	int Ret;
	int AddrLen;
 
	int RecvLen;
	char RecvBuf[1000];//服务器收到数据的缓冲区
 
	int ClientNum = -1;
	

	
	SocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == SocketServer)
	{
		printf("socket error!\n");
		return -1;
	}
 
	SocketServerAddr.sin_family      = AF_INET;
	SocketServerAddr.sin_port        = htons(SERVER_PORT);
 	SocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	
	Ret = bind(SocketServer, (const struct sockaddr *)&SocketServerAddr, sizeof(struct sockaddr));

	if (-1 == Ret)
	{
		printf("bind error!\n");
		return -1;
	}
  
	Ret = listen(SocketServer, BACKLOG);
	if (-1 == Ret)
	{
		printf("listen error!\n");
		return -1;
	}
 
	while (1)
	{
		AddrLen = sizeof(struct sockaddr);
		SocketClient = accept(SocketServer, (struct sockaddr *)&SocketClientAddr,(socklen_t *)&AddrLen);
		if (-1 != SocketClient)
		{
			ClientNum++;
			printf("Get connect from client %d : %s\n",  ClientNum, inet_ntoa(SocketClientAddr.sin_addr));
			if (!fork())
			{
				while (1)
				{
					/* 接收客户端发来的数据并显示出来 */
					RecvLen = recv(SocketClient, RecvBuf, 999, 0);
					if (RecvLen <= 0)
					{
						close(SocketClient);
						return -1;
					}
					else
					{
						ucRecvBuf[RecvLen] = '\0';
						printf("Get Msg From Client %d: %s\n", ClientNum, RecvBuf);
					}
				}				
			}
		}
	}
	
	close(SocketServer);
	return 0;
}

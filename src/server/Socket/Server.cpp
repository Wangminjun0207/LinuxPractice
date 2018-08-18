#include <stdio.h>
#include <string.h>
#include <sys/types.h> //包含<sys/types.h> 以访问 _LP64 和 _ILP32的定义         
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_PORT 8888 //端口号,方便直接修改

int main(){
	int SocketServer;
	int SocketClient;
	struct sockaddr_in SocketServerAddr; //服务器地址结构体
	struct sockaddr_in SocketClientAddr; //客户端地址结构体
	int Flag;
	int AddrLength; //地址结构体的大小
	int RecvLength; //接收数据的字节数
	char InforBuf[1000]; //服务器收到数据的缓冲区
	
	SocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(SocketServer == -1){
		printf("socket error!\n");
		return -1;
	}
	
	SocketServerAddr.sin_family  = AF_INET;           //协议族
	SocketServerAddr.sin_port    = htons(SERVER_PORT);//端口号
	SocketServerAddr.sin_addr.s_addr = INADDR_ANY;    //IP地址
	
	Flag = bind(SocketServer, (const struct sockaddr *)&SocketServerAddr, sizeof(struct sockaddr));
	//函数原型int blind(int sockfd,const struct sockaddr *addr,socklen_t addrlen)
	//上面函数第二个参数强制类型转换为通用的sockaddr结构
	if(Flag == -1){
		printf("bind error!\n");
		return -1;
	}
	Flag = listen(SocketServer, 1);
	if(Flag == -1){
		printf("listen error!\n");
		return -1;
	}
	
	while(1){
		AddrLength = sizeof(struct sockaddr);
		SocketClient = accept(SocketServer, (struct sockaddr *)&SocketClient, (socklen_t *)&AddrLength);
		//函数原型int accept(int sockfd, structsockaddr *addr, socklen_t *addrlen);
		if(SocketClient != -1){
			printf("Connect from client %s\n", inet_ntoa(SocketClientAddr.sin_addr));
			while(1){
				RecvLength = recv(SocketClient, InforBuf, 999, 0);
				if(RecvLength <= 0 ){
					close(SocketClient);
					return -1;
				}
				else{
					InforBuf[RecvLength] = '\0'; //字符串的最后添加结束符
					printf("Massage: %s\n", InforBuf);
				}
			}
		}
		close(SocketServer);
		return 0;
	}
}

#include <stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
#define SERVER_PORT 8888

int main(int argc, char **argv){
	int SocketClient;
	struct sockaddr_in SocketServerAddr;
	int Flag;
	char InforBuf[1000];//发送数据缓冲区
	int SendLength;//发送数据大小
	
	if(argc != 2){
		printf("%s <server_ip>\n",argv[0]);
		return -1;
	}
	
	SocketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == SocketClient)
	{
		printf("socket error!\n");
		return -1;
	}
	SocketServerAddr.sin_family      = AF_INET;
	SocketServerAddr.sin_port        = htons(SERVER_PORT);	
	if (0 == inet_aton(argv[1], &SocketServerAddr.sin_addr))//字符串IP转换为32位网络序列IP
 	{
		printf("invalid server_ip\n");
		return -1;
	}
	
	Flag = connect(SocketClient, (const struct sockaddr *)&SocketServerAddr, sizeof(struct sockaddr));	
	if (-1 == Flag)
	{
		printf("connect error!\n");
		return -1;
	}
	
	while (1)
	{
		if (fgets(InforBuf, 999, stdin))/*从stdin获得数据（我们自己实时敲入的）到InforBuf*/
		{
			SendLength = send(SocketClient, InforBuf, strlen(InforBuf), 0);
			if (SendLength <= 0)
			{
				close(SocketClient);
				return -1;
			}
		}
	}
	return 0;
}

#include <sys/types.h>        
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
 

#define SERVER_PORT 8888
 
int main(int argc, char **argv)
{
	int SocketClient;
	struct sockaddr_in SocketServerAddr;
	
	int Ret;
	char SendBuf[1000];
	int SendLen;

	
	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}
 
	SocketClient = socket(AF_INET, SOCK_STREAM, 0);
 
	SocketServerAddr.sin_family      = AF_INET;
	SocketServerAddr.sin_port        = htons(SERVER_PORT);
 	if (0 == inet_aton(argv[1], &SocketServerAddr.sin_addr))
 	{
		printf("invalid server_ip\n");
		return -1;
	}

	Ret = connect(SocketClient, (const struct sockaddr *)&SocketServerAddr, sizeof(struct sockaddr));	
	if (-1 == Ret)
	{
		printf("connect error!\n");
		return -1;
	}

	while (1)
	{
		if (fgets(SendBuf, 999, stdin)){
			SendLen = send(SocketClient, SendBuf, strlen(SendBuf), 0);
		}
	}
	
	return 0;
}

#include<common.h>

 //创建监听嵌套字
 //错误返回-1，正确返回嵌套字描述符。
int socket_create(int port)
{
	int sockfd;
	struct sockaddr_in sock_addr;

	//创建嵌套字
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket() error");
		return -1;
	}
	//设置本地嵌套字地址
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
	{
		close(sockfd);
		perror("setsockopt() error");
		return -1;
	}
	//绑定本地嵌套字到嵌套字
	if(bind(sockfd,(struct sockaddr *) &sock_addr,sizeof(sock_addr))<0)
	{
		perror("bind error");
		return -1;
	}
	if(listen(sockfd,5)<0)
	{
		perror("listen error\n");
		return -1;
	}
	return sockfd;

}
int socket_accept(int sock_listen)
{
	int sockfd;
	struct sockaddr_in client_addr;
	socklen_t=sizeof(client_addr);
	sockfd=accept(sock_listen,(struct sockaddr*)&client_addr,&len);

	if(sockfd<0)
	{
		perror("accept error");
		return -1;	
	}
	return sockfd;
}



int main()
{
	printf("hello world]\n");
return 0;
}



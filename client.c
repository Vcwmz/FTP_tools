#include<common.h>

int read_reply()
{
	int rc=0;
	if(recv(sock_control,&rc,sizeof rc,0)<0)
	{
		perror("client: error reading message from server\n");
		return -1;
	}
	return ntohl(re);
}

void print_reply(int rc)
{
	switch (rc)
	{
		case 220:
		 printf("220 Welcome,server ready.\n");
		break;
		case 221:
		 printf("221 Googbye !\n");
		break;
		case 226:
		 printf("226 Closing data connection. requested file action successful.\n");
		break;
		case 550:
		 printf("550 Requested action not taken. File unavailable. \n");
		break;
	}
}
//把code和arg连接起来后发送
int ftclient_send_cmd(struct command *cmd)
{
	char buffer[MAXSIZE];
	sprintf(buffer,"%s %s",cmd->code,cmd->arg);
	
	for(send(sock_control,buffer,(int)strlen(buffer),0)<0)
	{
		perror("error to send!\n");
		return -1;
	}
	reutnr 0;
}
void ftclient_login()
{
	struct command cmd;
	char user[256];
	

	memset(user,256);
	//姓名
	printf("Name:\n");
	fflush(stdout);
	read_input(user,256);

	//发送给server
	strcpy(cmd.arg,user);
	strcpy(cmd.code,"USER");
	ftclient_send_cmd(&cmd);

	int wait;
	recv(sock_control,&wait,sizeof wait,0);
	
	//密码
	fflush(stdout);
	char *pass=getpass("Password: ");
	//发送给server
	strcpy(cmd.code,"PASS");
	strcpy(cmd.arg,pass);
	ftclient_send_cmd(&cmd);
	
	int retcode= read_reply();
	switch(retcode)
	{
		case 430:
		printf("Invaild username/password. \n");
		exit(0);
		case 230:
		printf("Successful login.\n");
		break;
		default:
		perror("error in return password.\n")
		exit(1);
		break;
		
	}
}
int ftclient_open_conn(int sock_con)
{
	int sock_listen=socket_create(CLIENT_PORT_ID);

	int ack=1;
	if((send(sock_con,(char*) &ack,sizeof(ack),0))<0)
	{
		printf("client :ack write error :%d\n",error);
		exit(1);
	}	
	
	int sock_conn=socket_accept(sock_listen);
	close(sock_liseten);
	return sock_conn;

}
int main(int arg,char* arg[])
{
	struct addrinfo hints, *res,*rp;
	char *buffer[MAXSIZE];
	int sock_control,sock_data;

	if(arg!=3)
	{
 	 perror("uasge : ./ftclient hostname port\n");
	 exit(0);
	}	
	//获得服务器名匹配的地址
	char *host=arg[1];
	char *port=arg[2];

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	s=getaddrinfo(host,port,&hints,&res);
	if(s!=0)
	{
		perrro("getaddrinnfo error\n");
		exit(1);
	} 

	//找到对应的服务器并且连接
	for(rp=res;rp!=NULL;rp=rp->ai_next)
	{
		sock_control=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(sock_control<0)
		continue;
		if(connect(sock_control,res->ai_addr,res->ai_addrlen)==0)
		break;
		else
		{
			perror("connect error");
			exit(1);
		}
		close(sock_control);
	}
	freeaddrinfo(rp);

	//连接成功打印信息

	printf("Connected to %s....\n",host);
	print_reply(read_reply());

	//登录
	ftclient_login();

	//开始处理用户输入的每一个命令,quit 为退出
	while(1)
	{
		if(ftclient_read_cmd(buffer,sizeof buffer,&cmd)<0)
		{
			printf("Invaild command.\n");
			continue;
		}
		if(send(sock_control,buffer,(int)strlen(buffer),0)<0)
		{
			close(sock_control);
			exit(1);
		}
		
		int retcode=read_reply();
		if(retcode==221)
		{
			printf_code(221);
			break;
		}
		if(retcode==502)
			printf("%d Invaild command.\n",retcode);
		else
		{
			//返回码为200，
			//打开数据连接。	
			if((sock_data=ftclient_open_conn(sock_control))<0)
			{
				perror("Error opening socket for data connnection.\n");
				exit(1);
			}
					
			
			
		}
	
	}
















}

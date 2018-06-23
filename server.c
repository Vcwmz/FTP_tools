#include<stdio.h>
#include<stdlib.h>
int ftserver_recv_cmd(int sock_control,char *cmd,char *arg)
{
	char buffer[MAXSIZE];
	int rc=200;
	
	memset(buffer,0,MAXSIZE);
	memset(cmd,0,5);
	memset(arg,0,MAXSIZE);
	
	if((recv_data(sock_control,buffer,sizeof(buffer)))==-1)
	{
		perror("recv error.\n")
		return -1;
	}	

	strncpy(cmd,buffer,4);
	char *temp= buffer+5;
	strcpy(srg,temp);

	if(strcmp(cmd,"QUIT")==0)
		rc=231;
	else if (strcmp(cmd,"USER")==0 || (strcmp(cmd,"PASS"))==0 ||(strcmp(cmd,"LIST"))==0 ||strcmp(cmd,"RETR")==0)
		rc=200;
	else
 		tc=502;

	send_response(sock_control,rc);
	return rc;
}
void ftserver_process(int sock_control)
{
	int data_sock;
	char cmd[5];
	
	send_response(sock_control,220);

	if(ftserver_login(sock_control)==1)
		send_response(sock_control,230);
	else
	{
		send_response(sock_control,430);
		exit(0);
	}
	
	while(1)
	{
		int rc =ftserver_recv_cmd(sock_control,cmd,arg);
		
		if((rc<0)||(rc == 221))
			break;
		if(rc==200)
		{
			if((data_sock=ftserve_start_data_conn(sock_control))<0)
			{
				close(sock_control);
				exit(1);
			}
			if(strcmp(cmd,"LIST")==0)
			ftserver_list(data_sock,sock_control);
			else if(strcmp(cmd,"REST")==0)
			ftserver_retr(sock_control,data_sock,arg);

			close(data_sock);
		}
	}
}
int main(int arg,char *argv[])
{
	int sock_listen,sock_control;
	int port,pid;
	if(arg != 2)
	{
	   printf("Please enter the right information.\n");
	   exit(0);
	}

	port=atoi(argv[1]);

	if((sock_listen=socket_create(port))<0)	
	{
	   printf("Error creating.\n");
	   exit(1);
	}

	while(1)
	{
		if((sock_control=socket_accept(sock_lisiten))<0)
		{
			perror("socket of control was create error\n");
			break;
		}
		
		if((pid=fork())<0)
		 perror("error forking child process.\n");
		if else(pid==0)
		{
			close(sock_listen);
			ftserver_process(sock_control);
			close(sock_control);
			exit(0);
		}
	   close(sock_control);
	}
	close(sock_listen);
	return 0;
}

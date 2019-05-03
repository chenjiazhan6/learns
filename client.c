#include<stdio.h>
#include<netdb.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<fcntl.h>
#define MSG_SIZE 1024
int main(void)
{
	int sfd;
	char *node = "7bd9608bef8a";//这里存储的是主机名
    char *service = "10029";  //对方是2019号端口
	int s;
	struct addrinfo hints;
	struct addrinfo *result,*rp;


	memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;                                                                       hints.ai_protocol = 0;
	
	s = getaddrinfo(node,service,&hints,&result);
	
	if(s != 0)
	{
		fprintf(stderr,"getaddrinfo failed\n");
		exit(-1);
	}
	
	for(rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(sfd == -1)continue;
		if(connect(sfd,rp->ai_addr,rp->ai_addrlen) != -1)
		{
		    printf("it is a success connect\n");
			break;
		}
		close(sfd);
	}
	if(rp == NULL)
	{
		fprintf(stderr,"Could not connect\n");
		exit(-1);
	}
	freeaddrinfo(result);
	int flags = fcntl(sfd,F_GETFL,0);
	fcntl(sfd,F_SETFL,flags|O_NONBLOCK);
	fd_set wfds;
	fd_set rfds;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
//	char *msg = (char *)malloc(sizeof(char)*MSG_SIZE);
	//if(msg == NULL)
//	{
   //		printf("分配内存失败\n");
//		return -1;
//	}
//	//发送和接受信息的缓冲处
	char *rec_msg = (char *)malloc(sizeof(char) * MSG_SIZE);
	char *send_msg = (char *)malloc(sizeof(char *) * MSG_SIZE);
	while(1)
	{
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_SET(sfd,&rfds);
			FD_SET(0,&rfds);
			FD_SET(sfd,&wfds);
			int  retval = select(sfd+1,&rfds,&wfds,NULL,&tv);
			int msg_length = 0;
			int res;
		/**	if(retval == -1)
 			{
				perror("select()");
 			}
			else if(retval)
			{
				recv(sockfd,msg,MSG_SIZE,0);
				printf("%s\n",msg);
  			}
			else
			{
				printf("NO date within five second\n");	
  			}*/
			if(retval == 0)
			{
				printf("No date within five seconds\n");
			}
			else 
			{
				if(FD_ISSET(0,&rfds))
				{
					printf("开始你的输入");
					read(0,send_msg,1024);
					msg_length = strlen(send_msg);
					printf("%s\n",send_msg);
				}
				if(FD_ISSET(sfd,&wfds))
				{
					if(msg_length > 0)
					{
					    res = send(sfd,send_msg,strlen(send_msg),0);
						if(res != -1)
						{
							printf("send successful\n");
						}
						else
							printf("send failed.\n");
					}
				}
				if(FD_ISSET(sfd,&rfds))
				{
					res =  recv(sfd,rec_msg,MSG_SIZE,0);
					if(res != -1)
					{
						printf("接收到的信息:");
						printf("%s\n",rec_msg);
					}
				}		
	/*	int res = send(sfd,msg,strlen(msg),0);
		if(res == -1)
		{
			printf("send msg failed\n");
			return -1;
		}
		else
		{
			printf("successful send msg\n");
		}*/
		} 
	}

	return 0;//连接成功诶
		//需要搞清楚二级指针的使用诶
}

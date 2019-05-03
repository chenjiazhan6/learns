#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>
#include<time.h>
#include<fcntl.h>
#define MSG_SIZE 1024

typedef  struct _Peer_addr{
	int sockfd;
	char *msg;
	struct _Peer_addr *next;
}Peer_addr;
Peer_addr *head = NULL;
Peer_addr *last = NULL;
int main(int ac,char *args[])
{
	struct addrinfo hints;
//	printf("22line error\n");
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
//	printf("24error");
	int sfd,s;
	int sockfd;
	int connect_state = 0;
	int communicate_state = 0;
	int begin = 1;  //初始化状态
	time_t start_connect_time = 0;
	time_t stop_connect_time = 0;
	time_t now_time = 0;

	
//	printf("33line error\n");
	memset(&hints,0,sizeof(struct addrinfo));
//	printf("35line error\n");
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
//	printf("43line error\n");
//	printf("weisehnmehuichuxiancuowu1\n");
   //char *node = "7bd9608bef8a";
	char *server = "10029";
   
//	printf("46line error\n");
	s = getaddrinfo(NULL,server,&hints,&result);
//	printf("51line error\n");
//	printf("daodishinalichucuole1\n");
	if(s != 0)
 	{ 
		fprintf(stderr,"getaddrinfo error failed");
		return -1;
 	}
	int time = 1;
	printf("time\n");
    //sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);	
	for(rp = result; rp != NULL; rp = rp->ai_next)
	{ 
		sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(sfd == -1)
			continue;
		if(bind(sfd,rp->ai_addr,rp->ai_addrlen) == 0)
		{
			printf("success bind\n");
			break;
  		}
		close(sfd);
//		printf("%d\n",time);
  	}
	if(rp == NULL)
 	{
		printf("bind failed\n");
		return -1;
 	}
   //	flags = fcntl(sfd,F_GETFL,0);//获取文件的flag值
   // fcntl(sockfd,F_SETFL,flags|O_NONBLOCK);//设置为非阻塞模式
	if(listen(sfd,1) == -1)
 	{
	  /* if(errno == EADDRINUSE)
		   printf("1");
	   if(errno == EBADF)
		   printf("2");
	   if(errno == ENOTSOCK)
		   printf("3");
	   if(errno == EOPNOTSUPP)
  		   printf("4");*/
 		fprintf(stderr,"listen occur a error\n");
		return -1;
 	} 
	// 在主循环中不断接受发送过来的信息
/*	for(;;)
	{
		now_time = time(NULL);
		if(begin == 1 ||now_time - stop_connect_time >= 300)
		{
			printf("开始接受连接\n");
			connect_state = 1;
			start_connect_time = time(NULL);
		}
		if(connect_state == 1)
		{
			if(now_time - start_connect_time >= 10)
			{
				if(peer_head == NULL)	
				{
					start_connect_time = time(NULL);
					continue;
				}
			}
		}
  	}*/
	//成功接收到一个连接请求
	printf("开始接受请求:\n");
	sockfd = accept(sfd,NULL,NULL);
	printf("接收到一个请求\n");
    int flags = fcntl(sockfd,F_GETFL,0);
	fcntl(sockfd,F_SETFL,flags|O_NONBLOCK);//将这个sockfd设置为非阻塞
	if(sockfd != -1)
 	{
//    	char *buf = "it is a successful connect";
//		int length  = write(sockfd,buf,strlen(buf));
//		if(length == -1)
//		{
//			fprintf(stderr,"write error\n");
//			return -1;
//		}
 		fd_set rfds;
		fd_set wfds;
		struct timeval tv; 
		char *rec_msg = (char *)malloc(sizeof(char)*MSG_SIZE);
		char *send_msg = (char *)malloc(sizeof(char)*MSG_SIZE);
		if(rec_msg == NULL || send_msg == NULL)
 		{
			printf("malloc failed\n");
			return -1;
		}
		tv.tv_sec = 5;
		tv.tv_usec = 0;
	/*等待对面发送信息过来*/
		while(1)
 		{
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_SET(sockfd,&rfds);
			FD_SET(0,&rfds);
			FD_SET(sockfd,&wfds);
			int msg_length = 0;
			int res;
			int  retval = select(sockfd+1,&rfds,&wfds,NULL,&tv);
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
				if(FD_ISSET(sockfd,&wfds))
				{
					if(msg_length > 0)
					{
						res = send(sockfd,send_msg,strlen(send_msg),0);
						msg_length = 0;
						if(res != -1)
						{
							printf("send successful\n");
						}
						else
							printf("send failed.\n");
					}
				}
				if(FD_ISSET(sockfd,&rfds))
				{
					res = recv(sockfd,rec_msg,MSG_SIZE,0);
					if(res != -1)
					{
						printf("接收到的数据:");
						printf("%s\n",rec_msg);
					}
				}
			}
 		}
  	}	
	

}
/*
int  add_to_peer_addr(int sockfd)
{
	Peer_addr *peer = (struct _Peer_addr *)malloc(sizeof(struct _Peer_addr));
    peer->sockfd = sockfd;
	peer->msg = (char *)malloc(sizeof(char) * MSG_SIZE);
	peer->next = NULL;
	if(peer->msg == NULL)return -1;
	if(head == NULL)
	{
		head = peer;
		last = peer;
	}
	else
	{
		last->next = peer;
		last = peer;
	}
	return 1;
}*/

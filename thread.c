#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <netdb.h>

#include <linux/in.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/socket.h>



//#include "tcp_net_socket.h"
#include "define.h"
#include "list_operation.h"
#include "recvMsgHandle.h"
#include "cmd.h"
#include "getBR.h"
#include "IPMSG.h"
#include "scan_fun.h"
#include "listen_file.h"

//好友列表
struct user *users = NULL;

//接收文件链表
struct file *recvfilelist = NULL;

//发送文件链表
struct file *sendfilelist = NULL;

//昵称
char myname[20]; 

/*子线程1入口函数 */
int *thread_routine1(void *arg)
{
	int set = 1, recvbytes, sendBytes;// 设置BR_sock,否则可能导致sendto()失败  
	int addrLen = sizeof(struct sockaddr_in); 
	struct cmd cmd_obj;				
	
	char recvbuf[BUFF_SIZE];		//接收缓冲区
	
	
	//定义并配置  Lis_sock_addr , Recv_addr , send_sock_addr
	struct sockaddr_in Lis_sock_addr, Recv_addr;
	
	//Lis_sock初始化
	memset(&Lis_sock_addr, 0, sizeof(struct sockaddr_in));
	Lis_sock_addr.sin_family = AF_INET;  
    Lis_sock_addr.sin_port = htons(MSG_PORT);   //#define MSG_PORT  2425
    Lis_sock_addr.sin_addr.s_addr = INADDR_ANY; 
	
	memset(&Recv_addr, 0, sizeof(struct sockaddr_in)); 
	

	//定义并配置 BR_sock 及 Lis_sock	
	//BR_sock用于发送广播信息
	//Lis_sock用于接收来至MSG_PORT的UDP信息
	int  Lis_sock;

	if ((Lis_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) 
	{
            printf("create socket error\n");
            pthread_exit(NULL);
    }
	
	setsockopt(Lis_sock, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
	if(bind(Lis_sock, (struct sockaddr *)&Lis_sock_addr, sizeof(struct sockaddr)) == -1)
	{  
        printf("bind fail\n");  
        pthread_exit(NULL); 
    }  
	
	send_BR_ENTRY();
	
	while(1)
	{
		
		
		if((recvbytes = recvfrom(Lis_sock, recvbuf, 128, 0, 
				(struct sockaddr *)&Recv_addr, &addrLen)) != -1)
		{  
			recvbuf[recvbytes] = '\0';
			
			//printf("接收到数据包:%s 开始译码\n", recvbuf);
			
			memset(&cmd_obj, 0, sizeof(struct cmd));
			transcode(&cmd_obj, recvbuf, recvbytes);
			//printf("解析完命令开始执行\n");
			
			recvMsgHandle(&cmd_obj, &Recv_addr);	
			//printf("执行完成\n\n\n\n\n");
			
		}
		else
		{  
			printf("recvfrom fail\n");  
		}  
	
	}
	
}


int main(int argc, char *argv[])
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	
	//填充myname
	puts("请输入昵称:");
	fgets(myname, 20, stdin);
	myname[strlen(myname)-1]='\0';
	
	initUser();
    /*创建子线程1 */
	
    pthread_t t1, t2, t3;

    if (pthread_create(&t1, NULL, (void*)&thread_routine1, NULL)!=0) 
	{
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }
	
	if (pthread_create(&t2, NULL, (void*)&scan_fun, NULL)!=0) 
	{
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }
	
	
	if (pthread_create(&t3, NULL, (void*)&listen_file, NULL)!=0) 
	{
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }
	
	
    /*等待子线程1终止*/
	pthread_join(t2, NULL);
	
	pthread_cancel(t1);
    pthread_join(t1, NULL);
	
	pthread_cancel(t3);
    pthread_join(t3, NULL);

    
	
   

    pthread_exit(NULL);    
	
    return 0;
}



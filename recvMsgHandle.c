#include "recvMsgHandle.h"

/*
struct cmd
{
	char id[20];			//包编号
	char name[20];			//发送者姓名
	unsigned char cmdid;	//命令字
	char buf[1024];			//附加信息
};
*/


void showMsg(char * name, char *buf)
{
	printf("\n***************************************************************\n");
	printf("****\t来自 %s 的消息:\t\t\t\t   ****\n", name);
	printf("****\t%s\n", buf);
	printf("****                                                       ****\n");
	printf("***************************************************************\n");
}

void recvMsgHandle(struct cmd *msg ,struct sockaddr_in* send_addr)
{
	int send_sock, sendBytes, set = 1;
	
	if ((send_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
        printf("create socket error\n");
    }
	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	char buff[BUFF_SIZE];
	
	send_addr->sin_family = AF_INET;  
	send_addr->sin_port = htons(MSG_PORT); 
	
	
	
	//接受到用户广播上线信息
	if (GET_MODE(msg->cmdid ) == IPMSG_BR_ENTRY)
	{
		//没有此用户则添加
		if ( getUser(&send_addr->sin_addr) == NULL ) 
		{
			addUser(&send_addr->sin_addr, msg->name, msg->hostname);
		}
		
		
		//发送IPMSG_ANSENTRY信息
		coding(buff,IPMSG_ANSENTRY	, myname);
		
		
		if((sendBytes = sendto(send_sock, buff, strlen(buff), 0,  
            (struct sockaddr *)send_addr, sizeof(struct sockaddr))) == -1)
		{  
			printf("sendto fail\n");  
		}
	}
	
	//接收到应答上线信息
	if (GET_MODE(msg->cmdid )  == IPMSG_ANSENTRY)
	{
		//没有此用户则添加
		if ( getUser(&send_addr->sin_addr) == NULL ) 
		{
			addUser(&send_addr->sin_addr, msg->name, msg->hostname);
		}
	}
	
	//接收到用户下线信息
	if (GET_MODE(msg->cmdid ) == IPMSG_BR_EXIT)
	{
		//有此用户则删除
		struct user* logout_user;
		if ( (logout_user = getUser(&send_addr->sin_addr)) != NULL ) 
		{
			delUser(send_addr->sin_addr);
		}
	}
	
	
	//接收到消息
	if (GET_MODE(msg->cmdid )  == IPMSG_SENDMSG)
	{
		
		
		
		//IPMSG_SENDCHECKOPT		0x00000100UL
		if (( msg->cmdid & IPMSG_SENDCHECKOPT) == IPMSG_SENDCHECKOPT)
		{
			coding(buff,IPMSG_RECVMSG, msg->id);
			
			if((sendBytes = sendto(send_sock, buff, strlen(buff), 0,  
            (struct sockaddr *)send_addr, sizeof(struct sockaddr))) == -1)
			{  
				printf("sendto fail\n");  
			}
			
			//printf("IPMSG_RECVMSG发送完成\n");
		}

		if (( msg->cmdid & IPMSG_FILEATTACHOPT) == IPMSG_FILEATTACHOPT )
		{
			int fileid; 
			char filename[50];
			long size = 0, time = 0;
			
			sscanf(msg->buf,"%d:%[^:]:%ld:%ld:",&fileid, filename, &size, &time);
			addFile(fileid, filename, size, time, RECVFILELIST, &send_addr->sin_addr );
			printf("\n***************************************************************\n");
			printf("接收到来自%s的发送文件请求:%s\n", msg->name, filename);
			printf("***************************************************************\n");
		}
		else
		{
			//显示消息
			showMsg(msg->name, msg->buf);
		}
	}
	
	
	
	
}

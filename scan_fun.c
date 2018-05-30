#include "scan_fun.h"

void help_cmd()
{
	printf("\n***************************************************************\n");
	printf("\t显示好友列表:\t\tls\n");
	printf("\t给好友发送消息:\t\tsendto[空格]好友名\n");
	printf("\t显示发送文件列表:\tSFL\n");
	printf("\t显示接收文件列表:\tRFL\n");
	printf("\t发送文件:\t\tsendfile[空格]好友名\n");
	printf("\t接收文件:\t\tgetfile[空格]文件名\n");
	printf("\t退出程序:\t\t\\exit\n");
	printf("***************************************************************\n\n");
}

static int sendto_cmd(char *input_name)
{
	struct user *cur_user = getUser_name(input_name);
	
	if (cur_user == NULL)
	{
		printf("该用户不存在!\n");
		return 0;
	}
	
	char buff[BUFF_SIZE], codingbuff[BUFF_SIZE];
	
	int send_sock, set = 1, sendBytes, recvBytes;
	if ((send_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) 
		{
           		 printf("create socket error\n");
           		 pthread_exit(NULL);
    		}
	
	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	
	struct sockaddr_in send_addr;
	memset(&send_addr, 0, sizeof(struct sockaddr_in)); 
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(MSG_PORT);
	send_addr.sin_addr = cur_user->sin_addr;
	
	while(1)
	{
		printf("[to %s]:", cur_user->name );
		memset(&buff, 0, sizeof(buff)); 
		
		//接收到键盘信息
		
		if ((recvBytes = read(1, buff, sizeof(buff)) ) != -1 )
		{
			buff[recvBytes-1] = '\0';
			if ( strcmp(buff, "/exit") != 0)
			{
				if (recvBytes <= 1) continue;
				coding(codingbuff, IPMSG_SENDMSG, buff);
				if((sendBytes = sendto(send_sock, codingbuff, strlen(codingbuff), 0,  
						(struct sockaddr *)&send_addr, sizeof(struct sockaddr))) == -1)
				{  
					printf("sendto fail\n");  
				}
			}
			else
			{
				break;
			}
			
		}//read
		
	}
	
	
}

int sendfile_cmd(char *input_name)
{
	static unsigned int curr_id = 1;
	
	struct user *cur_user = getUser_name(input_name);
	
	if (cur_user == NULL)
	{
		printf("该用户不存在!\n");
		return 0;
	}
	
	struct stat filestat;
	
	char filename[BUFF_SIZE], codingbuff[BUFF_SIZE], append[BUFF_SIZE];
	
	
	//配置sock and addr 
	int send_sock, set = 1, sendBytes, recvBytes;
	if ((send_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
            printf("create socket error\n");
            pthread_exit(NULL);
    }
	
	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	
	struct sockaddr_in send_addr;
	memset(&send_addr, 0, sizeof(struct sockaddr_in)); 
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(MSG_PORT);
	send_addr.sin_addr = cur_user->sin_addr;
	
	
	//发送文件UDP消息
	printf("Please input file name:");
	if ((recvBytes = read(1, filename, sizeof(filename)) ) != -1 )
	{
		filename[recvBytes-1] = '\0';
		int retvalue = stat(filename, &filestat);
		if(retvalue < 0)
		{
			printf("文件名不正确！\n");
			return -1;
		}
		
		//将文件添加到发送文件列表
		addFile(curr_id++, filename, filestat.st_size, filestat.st_mtime, SENDFILELIST, (struct in_addr*)NULL);
		
		int index = recvBytes-2;
		while(filename[index] != '/')
		{
				index -= 1;
		}
		index += 1;
		//生成附加消息append 并发送
		sprintf(append,"%d:%s:%ld:%ld:", curr_id-1, &filename[index], filestat.st_size, filestat.st_mtime);
		
		coding(codingbuff, IPMSG_SENDMSG | IPMSG_FILEATTACHOPT | IPMSG_SENDCHECKOPT, append);
		if((sendBytes = sendto(send_sock, codingbuff, strlen(codingbuff), 0,  
				(struct sockaddr *)&send_addr, sizeof(struct sockaddr))) == -1)
		{  
			printf("sendto fail\n"); 
			return -1;
		}
	}
}

int  getfile_cmd(char *filename)
{
	/*
	struct file
	{
		char name[50];	//文件名
		int num;		// 文件序号
		long pkgnum;	//包编号
		long size;		//文件大小
		long ltime;		//最后修改时间
		struct in_addr sin_addr;	//发送者用户名
		struct file *next;
	};
	*/
	struct file *recv_file = getFile_name(filename,RECVFILELIST);
	if (recv_file == NULL)
	{
		printf("文件名错误！\n");
		return -1;
	}
	else
	{
		pthread_t       tid;

		pthread_attr_t  attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		
		pthread_create(&tid, &attr, (void*)&recv_file_thread, (void*)recv_file);
	}
}

void scan_fun()
{
	int recvBytes;
	
	char buf[BUFF_SIZE];
	
	while(1)
	{	
		printf("ipmsg> ");
		memset(&buf, 0, sizeof(buf)); 
		if ((recvBytes = read(1, buf, sizeof(buf))) != -1 ) 
		{
			buf[recvBytes-1] = '\0';
			//printf("recv:%s,%d\n\n", buf, recvBytes);
			
			if (strncmp(buf, "sendto", 6) == 0)
			{
				sendto_cmd(&buf[7]);
			}
			else if (strcmp(buf, "ls") == 0)
			{
				ls_cmd();
			}
			else if (strcmp(buf, "help") == 0)
			{
				help_cmd();
			}
			else if (strncmp(buf, "getfile", 7) == 0)
			{
				getfile_cmd(&buf[8]);
			}
			else if (strncmp(buf, "sendfile", 8) == 0)
			{
				sendfile_cmd(&buf[9]);
			}
			else if (strcmp(buf, "SFL") == 0)
			{
				ls_file (1);
			}
			else if (strcmp(buf, "RFL") == 0)
			{
				ls_file (0);
			}
			else if (strcmp(buf, "/exit") == 0)    
			{
				break;
			}
			
				
		}
		
	}
	
	send_BR_EXIT();
	
	pthread_exit(NULL);
}



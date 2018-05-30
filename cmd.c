#include "cmd.h"

/*
struct cmd
{
	char id[20];			//包编号
	char name[20];			//发送者姓名
	char hostname[20]; 		//机器名
	unsigned int cmdid;		//命令字
	char buf[1024];			//附加信息
};
*/

void transcode(struct cmd *obj, char *buf, int len)
{
	sscanf(buf, "1:%[^:]:%[^:]:%[^:]:%d:%[^\n]", obj->id, obj->name, obj->hostname, &obj->cmdid, obj->buf);
	//obj->cmdid = GET_MODE(obj->cmdid);
	//printf("译码结果: name:%s, hostname:%s, cmd:%d, buf:%s\n",obj->name, obj->hostname, obj->cmdid, obj->buf);
}

//编码 将要发送的信息编码为字符串保存到buf中
int coding(char *buf, unsigned int cmd, char *append)
{
	//获得本机机器名
	char hostname[20];
	gethostname(hostname, sizeof(hostname));
	
	//获得时间
	time_t h;  
	time(&h);//计算从1970到现在的秒数  
	
	if (append == NULL) append = "";
	
	sprintf(buf, "1:%ld:%s:%s:%d:%s",h, myname, hostname, cmd, append);
}


void send_BR_ENTRY()
{
	int BR_sock,set = 1,sendBytes ;
	
	struct sockaddr_in BR_sock_addr;
	
	char msg[BUFF_SIZE];
	
	if ((BR_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
        printf("create socket error\n");
    }
	setsockopt(BR_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	
	memset(&BR_sock_addr, 0, sizeof(struct sockaddr_in)); 
	BR_sock_addr.sin_family = AF_INET;
	BR_sock_addr.sin_port = htons(MSG_PORT);
	BR_sock_addr.sin_addr.s_addr = getBR().sin_addr.s_addr;
	
	coding(msg, IPMSG_BR_ENTRY, myname);
	if((sendBytes = sendto(BR_sock, msg, strlen(msg), 0,  
            (struct sockaddr *)&BR_sock_addr, sizeof(struct sockaddr))) == -1)
	{  
        printf("sendto fail\n");  
	}
	
}

void send_BR_EXIT()
{
	int BR_sock,set = 1,sendBytes ;
	
	struct sockaddr_in BR_sock_addr;
	
	char msg[BUFF_SIZE];
	
	if ((BR_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
        printf("create socket error\n");
    }
	setsockopt(BR_sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &set, sizeof(int)); 
	
	memset(&BR_sock_addr, 0, sizeof(struct sockaddr_in)); 
	BR_sock_addr.sin_family = AF_INET;
	BR_sock_addr.sin_port = htons(MSG_PORT);
	BR_sock_addr.sin_addr.s_addr = getBR().sin_addr.s_addr;
	
	coding(msg, IPMSG_BR_EXIT, NULL);
	if((sendBytes = sendto(BR_sock, msg, strlen(msg), 0,  
            (struct sockaddr *)&BR_sock_addr, sizeof(struct sockaddr))) == -1)
	{  
        printf("sendto fail\n");  
	}
}
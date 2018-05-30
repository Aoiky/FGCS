#ifndef _CMD_H_
#define _CMD_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>

#include <linux/in.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/socket.h>

#include "getBR.h"
#include "define.h"
#include "IPMSG.h"

extern char myname[20]; 

//修改cmd 为unsigned char类型 （命令字有32位 只提取出后八位即可）
struct cmd
{
	char id[20];			//包编号
	char name[20];			//发送者姓名
	char hostname[20]; 		//机器名
	unsigned int cmdid;		//命令字
	char buf[1024];			//附加信息
};

//版本号(忽略):  包编号(忽略) :发送者姓名 :  发送者机器名      : 命令字 : 附加信息
//	    1     :  1238605487   :    user   :      machine       :   32   :  hello

//译码 将接收到的字符串分解为struct cmd结构体 
void transcode(struct cmd *obj, char *buf, int len);

//编码 将要发送的信息编码为字符串保存到buf中
int coding(char *buf, unsigned int cmd, char *append);

//发送IPMSG_BR_ENTRY广播信息
void send_BR_ENTRY();

//发送IPMSG_BR_EXIT广播信息
void send_BR_EXIT();

#endif // _CMD_H_
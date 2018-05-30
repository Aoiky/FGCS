#ifndef _LIST_OPERATION_H_
#define _LIST_OPERATION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <netdb.h>
#include<malloc.h>  

#include <linux/in.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include "define.h"

extern struct user *users;
/*
struct user
{
	char name[20];		//发送者名称
	char host[20];  
	struct in_addr sin_addr;
	struct user *next;		
};
*/

//添加用户 sin_addr唯一主键  重复name在后面加序号 添加完成返回0 错误返回1
int addUser(struct in_addr* sin_addr, char name[], char hostname[]);

//根据sin_addr 删除用户  正确删除返回0 错误返回1
int delUser(struct in_addr sin_addr);

//根据sin_addr 获取user结构体 没有返回NULL
struct user* getUser(struct in_addr* sin_addr);

//根据ip 得到name
char* getName(struct in_addr* sin_addr);

//遍历所有用户 将用户列表（name, ip）输出
int ls_cmd(void);

struct user* getUser_name(char *name);

int initUser();//初始化


#endif // _LIST_OPERATION_H_
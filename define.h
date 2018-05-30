#ifndef _DEFINE_H_
#define _DEFINE_H_

#define MSG_PORT  2425
#define BUFF_SIZE 128
#define MAX_TCP_LISTEN_NUM 10

/*好友结构体*/
struct user
{
	char name[20];		//发送者名称
	char host[20];  
	struct in_addr sin_addr;
	struct user *next;		
};

/*文件结构体*/
struct file
{
	char name[50];	//文件名
	int num;		// 文件序号
	long pkgnum;	//包编号
	long size;		//文件大小
	long ltime;		//最后修改时间
	struct in_addr sin_addr;	//发送者ip
	struct file *next;
};

#endif // _DEFINE_H_
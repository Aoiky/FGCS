#include "file.h"
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

//接收文件链表
extern struct file *recvfilelist ;

//发送文件链表
extern struct file *sendfilelist ;

//添加结点到  接收0 或 发送1 文件列表
int addFile(int id, char* name, long size, long ltime, int flag, struct in_addr* p_addr)
{
	struct file *newfile = malloc(sizeof(struct file));
	if (newfile == NULL)
	{
		printf("malloc failed!\n");
		return -1;
	}
	memset(newfile, 0, sizeof(struct file));
	strncpy(newfile->name, name, 49);
	newfile->num = id;
	newfile->size = size;
	newfile->ltime = ltime;
	if (p_addr != NULL)
		newfile->sin_addr = *p_addr;
	
	
	if (flag == 0)
	{
		newfile->next = recvfilelist;
		recvfilelist = newfile;
	}
	else
	{
		newfile->next = sendfilelist;
		sendfilelist = newfile;
	}
}

//0 接收列表    1 发送列表
int ls_file (int flag)
{
	struct file *ret = NULL;
	if (flag == 0)
	{
		printf("recv file list:\n");
		ret = recvfilelist;
		while(ret != NULL)
		{
			printf("%s\n", ret->name);
			ret = ret->next;
		}
	}
	else
	{
		printf("send file list:\n");
		ret = sendfilelist;
		while(ret != NULL)
		{
			printf("%s\n", ret->name);
			ret = ret->next;
		}
	}
}

//0 接收列表    1 发送列表
int delFile(int num, int flag)
{
	struct file *ret = NULL, *temp = NULL;
	if (flag == 0)
	{
		ret = recvfilelist;
	}
	else
	{
		ret = sendfilelist;
	}
	
	if ( (ret != NULL) && (num == ret->num) )
	{
		temp = ret;
		ret = ret->next;
		free(temp);
	}
	else
	{
		while( ret->next != NULL )
		{
			if (num == ret->num)
			{
				temp = ret->next;
				ret = temp->next;
				free(temp);
			}
			ret = ret->next;
		}
	}
}


struct file* getFile(int num, int flag)
{
	struct file *ret = NULL;
	if (flag == 0)
	{
		ret = recvfilelist;
	}
	else
	{
		ret = sendfilelist;
	}
	

	while( ret != NULL )
	{
		if (num == ret->num)
		{
			return ret;
		}
		ret = ret->next;
	}
	return NULL;

}

struct file* getFile_name(char *name, int flag)
{
	struct file *ret = NULL;
	if (flag == 0)
	{
		ret = recvfilelist;
	}
	else
	{
		ret = sendfilelist;
	}
	

	while( ret != NULL )
	{
		if (strcmp(name, ret->name) == 0)
		{
			return ret;
		}
		ret = ret->next;
	}
	return NULL;

}
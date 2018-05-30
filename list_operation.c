#include "list_operation.h"

//添加用户 sin_addr唯一主键  重复name在后面加序号 添加完成返回0 错误返回1
int addUser(struct in_addr* sin_addr, char name[], char hostname[])//添加用用户
{
	
	struct user* newUser = NULL;
	struct user* rear = users;
	
	if ( (newUser = (struct user*)malloc(sizeof(struct user))) == NULL )
	{
		printf("newUser failed!\n");
		return 1;
	}
	
	newUser->sin_addr.s_addr = sin_addr->s_addr;
	strncpy(newUser->name, name, 20);
	strncpy(newUser->host, hostname, 20);

	while (rear->next != NULL)
	{
		rear = rear->next;
	}

	rear->next = newUser;
	newUser->next = NULL;

	return 0;
}

//根据sin_addr 删除用户  正确删除返回0 错误返回1
int delUser(struct in_addr sin_addr)//删除
{
	struct user* pre, *suc;
	pre = users;
	suc = users->next;
	while (suc->sin_addr.s_addr != sin_addr.s_addr)
	{
		pre = suc;
		suc = suc->next;
	}
	pre->next = suc->next;
	free(suc);
	//printf("删除成功！\n");

	return 0;
}

//根据sin_addr 获取user结构体 没有返回NULL
struct user* getUser(struct in_addr* sin_addr)//根据ip查找用户信息
{
	struct user* cur;
	cur = users;
	while ((cur->next != NULL)  && (cur->sin_addr.s_addr != sin_addr->s_addr))
	{
		cur = cur->next;
	}
	if (cur->sin_addr.s_addr == sin_addr->s_addr)
		return cur;
	else
		return NULL;

}

//根据用户名获取user结构体
struct user* getUser_name(char *name)
{
	struct user* cur;
	cur = users;
	while ((cur->next != NULL)  && (strcmp(name, cur->next->name) != 0))
	{
		cur = cur->next;
	}
	if (cur->next != NULL && strcmp(name, cur->next->name) == 0)
		return cur->next;
	else
		return NULL;

}

//根据ip 得到name
char* getName(struct in_addr* sin_addr)//根据ip查找用户的name
{
	int none = 0;
	
	struct user* cur;
	cur = users->next;
	while (cur->sin_addr.s_addr != sin_addr->s_addr)
	{
		cur = cur->next;
		if (cur->sin_addr.s_addr == sin_addr->s_addr)
			break;

		if (cur->next == NULL && cur->sin_addr.s_addr != sin_addr->s_addr)
		{
			none = 1;
			break;
		}
	}
	if (none == 0)
		return cur->name;
	else
		return NULL;
}
//遍历所有用户 将用户列表（name, ip）输出
int ls_cmd(void) //遍历
{
	struct user* cur = users->next;
	printf("\n***************************************************************\n");
	printf("****\t好友列表:                                          ****\n");
	while (cur != NULL)
	{
		printf("****\tname=%s", cur->name);
		printf("\tip=%s \t\t           ****\n", inet_ntoa(cur->sin_addr));
		cur = cur->next;
	}
	printf("***************************************************************\n");
	
	return 0;
}

int initUser()//初始化
{
	users = (struct user*)malloc(sizeof(struct user));
	if (users == NULL)
	{
		perror("申请空间失败！");
		return 1;
	}
	memset(users, 0 , sizeof(struct user));
	users->next = NULL;
	return 0;
}

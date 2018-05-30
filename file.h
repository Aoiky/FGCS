#ifndef _FILE_H_
#define _FILE_H_

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
#include <sys/stat.h>


#include "define.h"
#include "cmd.h"
#include "IPMSG.h"

#define RECVFILELIST  0
#define SENDFILELIST  1


int addFile(int id, char* name, long size, long ltime, int flag, struct in_addr* p_addr);
int ls_file (int flag);
int delFile(int num, int flag);
struct file* getFile(int num, int flag);
struct file* getFile_name(char*, int flag);

#endif // _FILE_H_
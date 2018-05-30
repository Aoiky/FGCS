#ifndef _RECVMSG_H_
#define _RECVMSG_H_

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

#include "cmd.h"
#include "list_operation.h"
#include "define.h"
#include "IPMSG.h"
#include "file.h"

extern char myname[20]; 

void recvMsgHandle(struct cmd* msg ,struct sockaddr_in*);

#endif // _RECVMSG_H_
#ifndef _SCAN_FUN_H_
#define _SCAN_FUN_H_

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

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/socket.h>

#include "define.h"
#include "list_operation.h"
#include "cmd.h"
#include "file.h"
#include "listen_file.h"

void scan_fun();

#endif // _scan_fun_H_
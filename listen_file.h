#ifndef _LISTEN_FILE_H_
#define _LISTEN_FILE_H_

#include<netinet/in.h>  // sockaddr_in  
#include<sys/types.h>   // socket  
#include<sys/socket.h>  // socket  
#include<stdio.h>       // printf  
#include<stdlib.h>      // exit  
#include<string.h>      // bzero 

#include<pthread.h>      //pthread_t , pthread_attr_t and so on.
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<unistd.h>    
#include <fcntl.h> 
#include <arpa/inet.h> 
#include <sys/utsname.h>

#include "cmd.h"
#include "file.h"

void listen_file();
void recv_file_thread(struct file * recv_file);

#endif
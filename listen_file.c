#include "listen_file.h"

static void send_file_thread(int *socket_fd)
{
	int file_id, recvbytes;
	struct cmd cmd_obj;
	
    char buff[BUFF_SIZE], Null[BUFF_SIZE];  
    bzero(buff, BUFF_SIZE);
	
    if((recvbytes = read(*socket_fd, buff, BUFF_SIZE)) < 0)  
    {  
        perror("Server Recieve Data Failed:");  
        pthread_exit(NULL);
    }  
  
	//解码接收到的命令
    transcode( &cmd_obj, buff, recvbytes);
    
	//命令不符合规范 错误返回
    if (GET_MODE(cmd_obj.cmdid ) != IPMSG_GETFILEDATA)
	{
		perror("TCP cmd fail:");  
		pthread_exit(NULL);
	}
    
	sscanf(cmd_obj.buf, "%[^:]:%d:%s", Null, &file_id, Null);
	struct file* send_file = getFile(file_id, SENDFILELIST);
  
	if (send_file == NULL)
	{
		printf("Send File:%d Not Found\n", file_id);  
		pthread_exit(NULL);
	}
	
    // 打开文件并读取文件数据  
    int file_fd = open(send_file->name, O_RDONLY);  
    if(file_fd == -1)  
    {  
        printf("File:%s Not Found\n", send_file->name);  
		pthread_exit(NULL);
    }  
    else  
    {  
		bzero(buff, BUFF_SIZE);  
        int length = 0;  
        // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止  
        while((length = read(file_fd, buff, BUFF_SIZE)) > 0)  
        {  
            if(write(*socket_fd, buff, length) < 0)  
            {  
                printf("Send File:%s Failed./n", send_file->name);  
                break;  
            }  
            bzero(buff, BUFF_SIZE);  
        }  
  
        // 关闭文件  
        close(file_fd);  
        //printf("File:%s Transfer Successful!\n", file_name);  
		
		delFile(file_id, SENDFILELIST);
    }  
	// 关闭与客户端的连接  
	close(*socket_fd);  
	free(socket_fd);
	pthread_exit(NULL);
}

void listen_file()
{
	struct sockaddr_in server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(MSG_PORT);  
  
    // 创建socket，若成功，返回socket描述符  
    int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);  
    if(server_socket_fd < 0)  
    {  
        perror("Create Socket Failed:");  
        pthread_exit(NULL);  
    }  
    int opt = 1;  
    setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  
  
    // 绑定socket和socket地址结构  
    if(-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))  
    {  
        perror("Server Bind Failed:");  
        pthread_exit(NULL);  
    }  
	
	if(-1 == (listen(server_socket_fd, MAX_TCP_LISTEN_NUM)))  
    {  
        perror("Server Listen Failed:");  
        pthread_exit(NULL);
    }  
      
    // socket监听
	while(1)  
    {  
        // 定义客户端的socket地址结构  
        struct sockaddr_in client_addr;  
        
		socklen_t client_addr_length = sizeof(struct sockaddr_in); 
  
        // 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信  
        // accept函数会把连接到的客户端信息写到client_addr中  
        int *new_server_socket_fd = (int*)malloc(sizeof(int));
		*new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length);  
        if(*new_server_socket_fd < 0)  
        {  
            perror("Server Accept Failed:");  
            break; 
        }  
		
		//创建接收线程
		pthread_t       tid;

		pthread_attr_t  attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		
		pthread_create(&tid, &attr, (void*)&send_file_thread, (void*)new_server_socket_fd);
		
    }    
	
}

void recv_file_thread(struct file * recv_file)
{
	
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr)); 
	client_addr.sin_family = AF_INET;    
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口
		
	int client_socket = socket(AF_INET,SOCK_STREAM,0);
	
	if( client_socket < 0)
	{
		printf("Create Socket Failed!\n");
		pthread_exit(NULL);
	}
		
	//把客户机的socket和客户机的socket地址结构联系起来
	if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
	{
		printf("Client Bind Port Failed!\n");
		pthread_exit(NULL);
	}
	
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = recv_file->sin_addr;
	server_addr.sin_port = htons(MSG_PORT);
	socklen_t server_addr_length = sizeof(server_addr);
		
	//链接Server
	if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
	{
		printf("Can Not Connect To Server!\n");
		pthread_exit(NULL);
	}
	
	char buff[BUFF_SIZE], append[BUFF_SIZE];
	bzero(buff,BUFF_SIZE);
	
	//编码 命令
	sprintf(append, " :%d: ",recv_file->num);
	coding(buff, IPMSG_GETFILEDATA, append);
	
	//发送 获取文件命令
	if ( write(client_socket, buff, sizeof(buff)) == -1 )
	{
		printf("Write TCP cmd failed!\n");
		pthread_exit(NULL);
	}
	
	//创建本地文件
	int host_file_fd = open(recv_file->name, O_WRONLY|O_CREAT);
	if (host_file_fd < 0)
	{
		printf("Create file failed!\n");
		pthread_exit(NULL);
	}
	
	int recvbytes = 0;
	while( (recvbytes = read(client_socket, buff, BUFF_SIZE)) > 0)
	{
		if ( write(host_file_fd, buff, recvbytes) < 0 )
		{
			printf("Write host_file failed!\n");
			pthread_exit(NULL);
		}
	}
	delFile(recv_file->num, RECVFILELIST);
	close(host_file_fd);
	close(client_socket);
}
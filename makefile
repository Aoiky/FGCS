obj = thread.o getBR.o recvMsgHandle.o list_operation.o cmd.o scan_fun.o file.o listen_file.o

thread: $(obj)
	gcc $(obj) -o thread -lpthread
	
thread.o: thread.c
	gcc -c thread.c -o thread.o 
	
getBR.o: getBR.c
	gcc -c getBR.c -o getBR.o 
	
recvMsgHandle.o: recvMsgHandle.c
	gcc -c recvMsgHandle.c -o recvMsgHandle.o
	
list_operation.o: list_operation.c
	gcc -c list_operation.c -o list_operation.o
	
cmd.o: cmd.c
	gcc -c cmd.c -o cmd.o
	
scan_fun.o : scan_fun.c
	gcc -o scan_fun.o -c scan_fun.c
	
file.o : file.c
	gcc -o file.o -c file.c
	
listen_file.o : listen_file.c
	gcc -o listen_file.o -c listen_file.c
	
clean:
	rm -f $(obj) thread 
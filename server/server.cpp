#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utilities.h"
#include "cmd_line.h"
#include "login.h"
using namespace std;

int server_init(int port, char* ip){
	int socket_fd = socket(AF_INET,SOCK_STREAM,0);

	if(socket_fd == -1){
		cout<<"socket failed"<<endl;
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	int res = bind(socket_fd,(struct sockaddr*)&addr,sizeof(addr));

	if(res == -1){
		cout<<"bind failed："<<endl;
		exit(-1);
	}
	cout<<"bind ok, waiting for client"<<endl;

	listen(socket_fd,100);

	return socket_fd;
}

void* server_echo(void* arg){
	int fd = *(int*)(&arg);
	if(fd == -1){
		cout<<"accept error"<<endl;
		exit(-1);
	}
	while(1){

	}
}


int main(){
	int socket_fd = server_init(5001, "127.0.0.1");

	char buff[100];
	getcwd(buff, 100);
	get_directory dir(buff);
	cmd_line c("");
	char l[3];
	char input[1024];
	string output;
	string temp;
	int s = 0;
	int fd = accept(socket_fd,(struct sockaddr*)NULL,NULL);
	c.set_fd(fd);
	while(1){
		char input[1024] = {0};
		output = dir.get_dir() + "$ ";
		write(fd, "o", 1);
		temp = str_length(output);
		write(fd, temp.c_str(), temp.length());
		write(fd, output.c_str(), output.length());


		write(fd, "i", 1);
		read(fd, l, 3);
		s = atoi(l);
		cout<<s<<endl;
		read(fd, input, s);
		cout<<input<<endl;
		temp = input;
		c.set_input(temp);
		c.directory_cmd(&dir);
		c.file_cmd(&dir);

	}
	// 
	// for(int i = 0; i < 100; i++){
	// 	int fd = accept(socket_fd,(struct sockaddr*)NULL,NULL);
	// 	cout<<"Connected to client "<<fd<<endl;
	// 	pthread_create(&t[i],NULL,server,(void *)(long)fd);
	// }
	// login l;
	// l.login_page();

/*
	string t;
	getline(cin, t);
	cout<<str2int(t.c_str())<<endl;
	*/
}
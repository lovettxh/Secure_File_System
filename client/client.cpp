#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include "utilities.h"

using namespace std;

int client_init(int port, char* ip){
	int socket_fd = socket(AF_INET, SOCK_STREAM,0);
	if(socket_fd == -1)
	{
		cout<<"socket failed"<<endl;
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int res = connect(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
	if(res == -1)
	{
		cout<<"bind failed"<<endl;
		exit(-1);
	}
	cout<<"bind success"<<endl;
	return socket_fd;
}

int main(){
	int socket_fd = client_init(5001, "127.0.0.1");
	char indicator;
	int s = 0;
	char l[3];
	char output[1024];
	string input="";
	while(1){
		read(socket_fd, &indicator, 1);
		char output[1024] = {0};
		if(indicator == 'o'){
			read(socket_fd, l, 3);
			s = atoi(l);
			cout<<s<<endl;
			read(socket_fd, output, s);
			cout<<output;
		}else if(indicator == 'i'){

			getline(cin, input);
			write(socket_fd, str_length(input).c_str(), str_length(input).length());
			write(socket_fd, input.c_str(), input.length());
		}
	}
	return 0;
}
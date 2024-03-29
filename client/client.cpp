#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "utilities.h"
#include "login.h"

using namespace std;

// client initialization
int client_init(int port, char* ip){
	int socket_fd = socket(AF_INET, SOCK_STREAM,0);		// setting up socket
	if(socket_fd == -1)
	{
		cout<<"socket failed"<<endl;
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);						// setting up port number
	addr.sin_addr.s_addr = inet_addr(ip);				// setting up ip address
	int res = connect(socket_fd,(struct sockaddr*)&addr,sizeof(addr));	// bind to socket
	if(res == -1)
	{
		cout<<"bind failed"<<endl;
		exit(-1);
	}
	cout<<"bind success"<<endl;
	return socket_fd;
}

// client side SFS page handling function
// receive signal 'i' or 'o' from server to perform simple input or output to user
int SFS_page(int socket_fd){
	char indicator;
	int s = 0;
	char l[3];
	char output[1024];
	string input="";
	while(1){
		read(socket_fd, &indicator, 1);
		char output[1024] = {0};
		if(indicator == 'o'){				// when client receives 'o', it means server needs it to output something
			read(socket_fd, l, 3);
			s = atoi(l);
			read(socket_fd, output, s);
			cout<<output;
		}else if(indicator == 'i'){			// when client receives 'i', it means server needs user input
			getline(cin, input);
			write(socket_fd, str_length(input).c_str(), 3);
			write(socket_fd, input.c_str(), input.length());
			if(!(split(input, " ")[0].compare("logout"))){
				return 1;
			}else if(!(split(input, " ")[0].compare("exit"))){
				return 2;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[]){
	if(argc != 3){
		cout<<"Invalid command"<<endl;
		cout<<"should be: ./server (ip address) (port number)"<<endl;
		exit(1);
	}
	int socket_fd = client_init(atoi(argv[2]), argv[1]);
	login l;
	int a;
	l.set_fd(socket_fd);
	while(1){
		l.login_page();
		a = SFS_page(socket_fd);
		if(a == 2){
			break;
		}
	}
	return 0;
}
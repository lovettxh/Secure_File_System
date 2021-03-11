#include <iostream>
#include <fstream>
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

string entering_page(int fd){
	char mode;
	char l[3];
	char g[1024];
	fstream file;
	string temp;
	int s;
	while(1){
		read(fd, &mode, 1);
		cout<<mode<<endl;
		if(mode == '1'){
			file.open("g.dat", ios::in);
			while(getline(file, temp)){
				write(fd, str_length(temp).c_str(), 3);
				write(fd, temp.c_str(), temp.length());
			}
			write(fd, "000", 3);
			file.close();

		}else if(mode == '2'){
			file.open("g.dat", ios::out);
			s = 1;
			while(s){
				char g[1024] ={0};
				read(fd, l, 3);
				s = atoi(l);
				if(s){
					read(fd, g, s);
					temp = g;
					file<<temp<<endl;
				}
			}
			file.close();

		}else if(mode == '3'){
			file.open("l.dat", ios::in);
			while(getline(file, temp)){
				write(fd, str_length(temp).c_str(), 3);
				write(fd, temp.c_str(), temp.length());
			}
			write(fd, "000", 3);
			file.close();

		}else if(mode == '4'){
			file.open("l.dat", ios::out);
			s = 1;
			while(s){
				char g[1024] = {0};
				read(fd, l, 3);
				s = atoi(l);
				if(s){
					read(fd, g, s);
					temp = g;
					file<<temp<<endl;
				}
			}
			file.close();
		}else if(mode == '0'){
			char c;
			read(fd, &c, 1);
			if(c == 't')
				break;
		}
	}
	char out[1024] = {0};
	read(fd, l, 3);
	s = atoi(l);
	read(fd, out, s);
	temp = out;
	return temp;
}

void SFS_page(get_directory dir, cmd_line c, int fd){
	string output;
	string temp;
	char l[3];
	char input[1024];
	int s = 0;
	while(1){
		char input[1024] = {0};
		output = dir.user_dir() + "$ ";
		write(fd, "o", 1);
		temp = str_length(output);
		write(fd, temp.c_str(), 3);
		write(fd, output.c_str(), output.length());
		write(fd, "i", 1);
		read(fd, l, 3);
		s = atoi(l);
		cout<<l<<endl;
		read(fd, input, s);
		cout<<input<<endl;
		temp = input;
		c.set_input(temp);
		c.directory_cmd(&dir);
		c.file_cmd(&dir);

	}
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
	
	string u;

	int socket_fd = server_init(5000, "127.0.0.1");

	char buff[100];
	getcwd(buff, 100);
	get_directory dir(buff);

	// while(1){
	// 	cout<<dir.get_dir()<<"  $";
	// 	cin>>u;
	// 	cout<<dir.check_exist(u)<<endl;
	// }

	cmd_line c("");
	
	int fd = accept(socket_fd,(struct sockaddr*)NULL,NULL);
	u = entering_page(fd);
	c.set_fd(fd);
	dir.set_user(u);
	SFS_page(dir, c, fd);

	
	// for(int i = 0; i < 100; i++){
	// 	int fd = accept(socket_fd,(struct sockaddr*)NULL,NULL);
	// 	cout<<"Connected to client "<<fd<<endl;
	// 	pthread_create(&t[i],NULL,server,(void *)(long)fd);
	// }
	// login l;
	// l.login_page();
}
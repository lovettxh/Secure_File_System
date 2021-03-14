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

#include "security.h"
using namespace std;


// initialize the server socket for communicating with client
int server_init(int port, char* ip){
	
	int socket_fd = socket(AF_INET,SOCK_STREAM,0);	// setting up socket
	if(socket_fd == -1){
		cout<<"socket failed"<<endl;
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);			// setting up port number
	addr.sin_addr.s_addr = inet_addr(ip);	// setting up ip address
	int res = bind(socket_fd,(struct sockaddr*)&addr,sizeof(addr));		// bind to the socket 
	if(res == -1){
		cout<<"bind failed："<<endl;
		exit(-1);
	}
	cout<<"bind ok, waiting for client"<<endl;

	listen(socket_fd,100);					// waiting for client
	return socket_fd;
}

// function for dealing with login/sign in page on server side
// It will return the user ID as a string when authentication is complete
string entering_page(int fd){
	char mode;
	char l[3];
	char g[1024];
	fstream file;
	string temp;
	int s;
	while(1){
		read(fd, &mode, 1);
		// mode '1' is for reading group information
		if(mode == '1'){
			file.open("g.dat", ios::in);		
			while(getline(file, temp)){			// read from file
				write(fd, str_length(temp).c_str(), 3);
				write(fd, temp.c_str(), temp.length());
			}
			write(fd, "000", 3);
			file.close();

		// mode '2' is for saving group information
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
					file<<temp<<endl;	// write to file
				}
			}
			file.close();

		// mode '3' is for reading user information 
		}else if(mode == '3'){
			file.open("l.dat", ios::in);
			while(getline(file, temp)){			// read from file
				write(fd, str_length(temp).c_str(), 3);
				write(fd, temp.c_str(), temp.length());
			}
			write(fd, "000", 3);
			file.close();

		// mode '4' is for saving user information
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
					file<<temp<<endl;	// write to file
				}
			}
			file.close();

		// mode 't' stands for 'true' that indicates authentication is complete
		}else if(mode == 't'){
			break;
		}
	}
	char out[1024] = {0};
	read(fd, l, 3);
	s = atoi(l);
	read(fd, out, s);	// read user ID
	temp = out;
	return temp;
}

// this function is used for dealing all the command line action in the file system
// It returns a integer indicate whether it's a exit or logout
int SFS_page(get_directory dir, cmd_line c, int fd){
	string output;
	string temp;
	char l[3];
	char input[1024];
	int s = 0;
	int a = 0;
	while(1){
		char input[1024] = {0};
		output = dir.user_dir() + "$ ";		// output user current directory
		write(fd, "o", 1);
		temp = str_length(output);
		write(fd, temp.c_str(), 3);
		write(fd, output.c_str(), output.length());
		write(fd, "i", 1);
		read(fd, l, 3);
		s = atoi(l);
		read(fd, input, s);					// read user command line input from client side
		temp = input;
		c.set_input(temp);					// setting up command line input

		// function for dealing with different command line input
		c.directory_cmd(&dir);
		c.file_cmd(&dir);
		a = c.exit_cmd(&dir);

		if(a != 0){							// a != 0 indicate exit or logout
			return a;
		}
	}
}

int main(){
	
	string u;
	int a;
	int socket_fd = server_init(5001, "127.0.0.1");		// initialize server
	char buff[100];
	getcwd(buff, 100);									// get current directory
	get_directory dir(buff);							// get_directory class is used for searching all the containing given the directory
	cmd_line c("");										// cmd_line class is used for dealing with all command line inputs
	int fd = accept(socket_fd,(struct sockaddr*)NULL,NULL);	//waiting for client
	while(1){
		u = entering_page(fd);						// login page
		c.set_fd(fd);
		dir.set_user(u);							// setup user name
		check_file_integrity(&dir, fd);				// check file integrity
		user_decrypt(&dir, (dir.get_home_dir() + "/" + dir.get_user()));	// decrypt all user files
		a = SFS_page(dir, c, fd);					// SFS page

		if(a == 1){									// a == 1 for exit
			user_encrypt(&dir, (dir.get_home_dir() + "/" + dir.get_user()));
			save_file_integrity(&dir);
			break;
		}else if(a == 2){							// a == 2 for logout
			user_encrypt(&dir, (dir.get_home_dir() + "/" + dir.get_user()));
			save_file_integrity(&dir);
			dir.set_dir(dir.get_home_dir());
		}
	}
	

	
}
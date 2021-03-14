#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "cmd_line.h"

using namespace std;

// constructor to setup the first cmd input 
cmd_line::cmd_line(string a){
	this->cmd_input = a;
	this->cmd_set = split(this->cmd_input, " ");
}
// setup socket_fd for communication
void cmd_line::set_fd(int fd){
	this->fd = fd;
}
// setup new command input
void cmd_line::set_input(string a){
	this->cmd_input = a;
	this->cmd_set = split(this->cmd_input, " ");
}

// function for dealing command line input
void cmd_line::directory_cmd(get_directory* dir){
	if(this->cmd_set.empty()){			// check empty
		return;
	}
	switch(str2int(this->cmd_set[0].c_str())){

		case str2int("ls"):{				// ls: list all file and folder under current directory
			dir->search_fileSet();
			vector<string> temp_fileSet = dir->get_fileSet();
			string out = "";
			for(const auto a:temp_fileSet){
				out += (a + "     ");
			}
			out+="\n";
			write(this->fd, "o",1);
			write(this->fd, str_length(out).c_str(), str_length(out).length());
			write(this->fd, out.c_str(), out.length());
			break;
		}

		case str2int("cd"):{				// cd + </directory>: change to given directory
			string new_dir = "";
			string message;
			if(this->cmd_set.size() != 2){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			if(!this->cmd_set[1].compare("~")){			// cd ~: for returning to home directory
				dir->set_dir(dir->get_home_dir());
				break;

			}else if(!this->cmd_set[1].compare("..")){	// cd ..: for return to previous directory
				if(!dir->get_dir().compare(dir->get_home_dir())){
					break;
				}
				vector<string> dir_temp = split(dir->get_dir(), "/");
				dir_temp.pop_back();
				
				for(const auto a:dir_temp){
					if(a.compare(""))
						new_dir += ("/" + a);
				}
				dir->set_dir(new_dir);
				break;
			}

			this->remove_slash();
			char t = dir->check_exist(this->cmd_set[1]);
			if(t == 'n'){
				message = "Directory not found\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}else if(t == 'f'){
				message = "Target is not a directory\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd ,message.c_str(), message.length());
				break;
			}
			new_dir = dir->get_dir() + "/" + this->cmd_set[1];
			dir->set_dir(new_dir);
			break;
		}

		
		case str2int("mkdir"):{			// mkdir + <directory_name>: create a folder in the current directory
			string message;
			if(this->cmd_set.size() != 2){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			this->remove_slash();
			char t = dir->check_exist(this->cmd_set[1]);
			if(t != 'n'){
				message ="File already existed\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}

			string dir_path = dir->get_dir() + "/" +this->cmd_set[1];
			if(opendir(dir_path.c_str())==NULL){
				if(mkdir(dir_path.c_str(), 0777)){
					message = strerror(errno);
					message = "Error: " + message +"\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());
				}else{
					message = "Directory Created\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());
				}
			}else{
				message = "Directory already exist\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
			}
			break;
		}

		case str2int("rm"):{				// rm + <name>: delete file or folder
											// delete a folder will delete everything inside the folder
			string new_path;
			string message;
			if(this->cmd_set.size() != 2){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			this->remove_slash();
			char t = dir->check_exist(this->cmd_set[1]);
			if(t == 'n'){
				message = "File\\Directory not found\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			if(t == 'd'){
				this->delete_dir(dir, "/"+this->cmd_set[1]);
			}else{
				new_path = dir->get_dir() + "/" + this->cmd_set[1];
				if(remove(new_path.c_str())){
					perror("Error");
				}else{
					message = "File deleted\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());
				}
			}
			break;
		}
	}
}

// function for dealing command line input
void cmd_line::file_cmd(get_directory* dir){
	if(this->cmd_set.empty()){			// check empty
		return;
	}
	switch(str2int(this->cmd_set[0].c_str())){
		case str2int("touch"):{			// touch + <file_name>: create an empty file given its name
			string message;
			string file_dir;
			if(this->cmd_set.size() != 2){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			if(isdigit(this->cmd_set[1][0]) || isalpha(this->cmd_set[1][0])){
				file_dir = dir->convert_dir(this->cmd_set[1]);
				ofstream file {file_dir};
			}else{
				message = "Invalid file name\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
			}
			break;
		}

		case str2int("cat"):{		// cat + <file_name>: read file content and display it
			string message;
			if(this->cmd_set.size() != 2){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			this->remove_slash();
			char t = dir->check_exist(this->cmd_set[1]);
			if(t != 'n'){
				if(t == 'f'){
					string file_dir = dir->get_dir() + "/" + this->cmd_set[1];
					fstream file;
					file.open(file_dir, ios::in);
					string temp;
					while(getline(file, temp)){
						temp+="\n";
						write(this->fd,"o",1);
						write(this->fd, str_length(temp).c_str(), str_length(temp).length());
						write(this->fd, temp.c_str(), temp.length());
					}
					file.close();
				}else{
					message = "Target cannot be a directory\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());					
				}
			}else{
				message = "File not found\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());				
			}
			break;
		}

		case str2int("echo"):{		// echo + "content" >> <file_name>: write the content to the file
									// If the file already exist, it will append at the file end.
									// If the file doesn't exist, it will create the file and write into file.
			string message;
			if(this->cmd_set.size() < 3){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			size_t start = this->cmd_input.find_first_of("\"");
			size_t end = this->cmd_input.find_last_of("\"");
			string w = this->cmd_input.substr(start + 1, end-start-1);
			w = string_handle(w);
			string file_dir;
			this->remove_slash();
			char t = dir->check_exist(this->cmd_set.back());
			if(t == 'n'){
				if(isdigit(this->cmd_set.back()[0]) || isalpha(this->cmd_set.back()[0])){
					file_dir = dir->get_dir() + "/" + this->cmd_set.back();
					ofstream file {file_dir};
					ofstream file1;
					file1.open(file_dir, ios::out);
					file1<<w;
					file1.close();
					message = "File created\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());
				}else{
					message = "Invalid file name\n";
					write(this->fd,"o",1);
					write(this->fd, str_length(message).c_str(), str_length(message).length());
					write(this->fd, message.c_str(), message.length());
				}
			}else if(t == 'f'){
				file_dir = dir->convert_dir(this->cmd_set.back());
				ofstream file1;
				file1.open(file_dir, ios::out | ios::app);
				file1<<w;
				file1.close();
			}else{
				message = "Target is a directory\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
			}
			break;
		}

		case str2int("mv"):{		// "mv" + <old_name/location> + <new_name/location>: move a file to new location
									// It can also be used to rename file or folder
			string message;
			if(this->cmd_set.size() != 3){
				message = "Invalid command\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
				break;
			}
			this->remove_slash();
			char t = dir->check_exist(this->cmd_set[1]);
			if(t == 'n'){
				message = "Invalid source address\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
			}
			string old_path = dir->convert_dir(this->cmd_set[1]);
			string new_path = dir->convert_dir(this->cmd_set[2]);
			int result = rename(old_path.c_str(), new_path.c_str());
			if(result){
				message = strerror(errno);
				message += "\n";
				write(this->fd,"o",1);
				write(this->fd, str_length(message).c_str(), str_length(message).length());
				write(this->fd, message.c_str(), message.length());
			}
			break;
		}
	}
}

// function for dealing command line input
int cmd_line::exit_cmd(get_directory* dir){
	if(this->cmd_set.empty()){			// check empty
		return 0;
	}
	switch(str2int(this->cmd_set[0].c_str())){
		case str2int("logout"):{		// logout
			return 2;
		}
		case str2int("exit"):{			// exit
			return 1;
		}
	}
	return 0;
}

void cmd_line::remove_slash(){
	for(int i = 0; i < this->cmd_set.size(); i++){
		if(this->cmd_set[i][0]=='/'){
			this->cmd_set[i] = this->cmd_set[i].substr(1);
		}
	}
	return;
}


void cmd_line::delete_dir(get_directory* dir, string dir_name){
	string message;
	string new_path = dir->get_dir() + dir_name;
	string old_path = dir->get_dir();
	string temp_path;
	dir->set_dir(new_path);
	dir->search_fileSet();
	vector<string> temp_fileSet = dir->get_fileSet();
	for(const auto a:temp_fileSet){
		if(a[0]=='/'){
			this->delete_dir(dir, a);
		}else{
			temp_path = new_path + "/" + a;
			if(remove(temp_path.c_str())){
				perror("Error when deleting file");
				break;
			}
		}
	}
	if(rmdir(new_path.c_str())){
		message = strerror(errno);
		message = "Directory delete error: " + message + "\n";
		write(this->fd,"o",1);
		write(this->fd, str_length(message).c_str(), str_length(message).length());
		write(this->fd, message.c_str(), message.length());
	}
	dir->set_dir(old_path);
	return;
}
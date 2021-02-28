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
#include "utilities.h"

using namespace std;

cmd_line::cmd_line(string a){
	this->cmd_input = a;
	this->cmd_set = split(this->cmd_input, " ");
}



void cmd_line::set_input(string a){
	this->cmd_input = a;
	this->cmd_set = split(this->cmd_input, " ");
}



void cmd_line::directory_cmd(get_directory* dir){
	if(this->cmd_set.empty()){
		return;
	}
	switch(str2int(this->cmd_set[0].c_str())){

		case str2int("ls"):{
			dir->search_fileSet();
			vector<string> temp_fileSet = dir->get_fileSet();
			for(const auto a:temp_fileSet){
				cout<<a<<"     ";
			}
			cout<<endl;
			break;
		}

		case str2int("cd"):{
			string new_dir = "";
			if(!this->cmd_set[1].compare("~")){
				dir->set_dir(dir->get_home_dir());
				break;
			}else if(!this->cmd_set[1].compare("..")){
				if(!dir->get_dir().compare(dir->get_home_dir())){
					break;
				}
				vector<string> dir_temp = split(dir->get_dir(), "/");
				dir_temp.pop_back();
				
				for(const auto a:dir_temp){
					new_dir += ("/" + a);
				}
				dir->set_dir(new_dir);
				break;
			}


			if(!this->check_exist(dir, this->cmd_set[1])){
				cout<<"Directory not found"<<endl;
				break;
			}else if((this->cmd_set[1][0])!='/'){
				cout<<"Target is not a directory"<<endl;
				break;
			}
			new_dir = dir->get_dir() + this->cmd_set[1];
			dir->set_dir(new_dir);
			break;
		}

		
		case str2int("mkdir"):{
			if((this->cmd_set[1][0])!='/'){
				cout<<"Invalid Directory name"<<endl;
				break;
			}
			string dir_path = dir->get_dir() + this->cmd_set[1];
			
			if(opendir(dir_path.c_str())==NULL){
				if(mkdir(dir_path.c_str(), 0777)){
					cout<<"Error: "<<strerror(errno)<<endl;
				}else{
					cout<<"Directory Created"<<endl;
				}
			}else{
				cout<<"Directory already exist"<<endl;
			}
			break;
		}

		case str2int("rm"):{
			string new_path;
			if(!this->check_exist(dir, this->cmd_set[1])){
				cout<<"File\\Directory not found"<<endl;
				break;
			}
			if((this->cmd_set[1][0])=='/'){
				this->delete_dir(dir, this->cmd_set[1]);

			}else{
				new_path = dir->get_dir() + "/" + this->cmd_set[1];
				if(remove(new_path.c_str())){
					perror("Error");
				}else{
					cout<<"File deleted"<<endl;
				}
			}
			break;
		}
	}
}

void cmd_line::file_cmd(get_directory* dir){
	if(this->cmd_set.empty()){
		return;
	}
	switch(str2int(this->cmd_set[0].c_str())){
		case str2int("touch"):{
			string file_dir;
			if(isdigit(this->cmd_set[1][0]) || isalpha(this->cmd_set[1][0])){
				file_dir = dir->get_dir() + "/" + this->cmd_set[1];
				ofstream file {file_dir};
			}else{
				cout<<"Invalid file name"<<endl;
			}
			break;
		}

		case str2int("cat"):{
			if(check_exist(dir, this->cmd_set[1])){
				if(this->cmd_set[1][0]!='/'){
					string file_dir = dir->get_dir() + "/" + this->cmd_set[1];
					fstream file;
					file.open(file_dir, ios::in);
					string temp;
					while(getline(file, temp))
						cout<<temp<<endl;
					file.close();

				}else{
					cout<<"Target cannot be a directory"<<endl;
				}
			}else{
				cout<<"File not found"<<endl;
			}
			
			break;
		}

		case str2int("echo"):{
			size_t start = this->cmd_input.find_first_of("\"");
			size_t end = this->cmd_input.find_last_of("\"");
			string w = this->cmd_input.substr(start + 1, end-start-1);
			w = string_handle(w);
			string file_dir;
			if(!this->check_exist(dir, this->cmd_set.back())){
				if(isdigit(this->cmd_set.back()[0]) || isalpha(this->cmd_set.back()[0])){
					file_dir = dir->get_dir() + "/" + this->cmd_set.back();
					ofstream file {file_dir};
					ofstream file1;
					file1.open(file_dir, ios::out);
					file1<<w;
					file1.close();
					cout<<"file created"<<endl;
				}else{
					cout<<"Invalid file name"<<endl;
				}
			}else{
				file_dir = dir->get_dir() + "/" + this->cmd_set.back();
				ofstream file1;
				file1.open(file_dir, ios::out | ios::app);
				file1<<w;
				file1.close();
			}
			break;
		}
	}
}



bool cmd_line::check_exist(get_directory* dir, string name){
	dir->search_fileSet();
	vector<string> temp_fileSet = dir->get_fileSet();
	for(const auto a:temp_fileSet){
		if(!name.compare(a)){
			return true;
		}
	}
	return false;
}

void cmd_line::delete_dir(get_directory* dir, string dir_name){
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
				cout<<temp_path<<endl;
				perror("Error when deleting file");
				break;
			}
		}
	}
	if(rmdir(new_path.c_str())){
		cout<<"Directory delete error: "<<strerror(errno)<<endl;
	}
	dir->set_dir(old_path);
	return;
}
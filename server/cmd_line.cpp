#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "cmd_line.h"

using namespace std;

constexpr unsigned int str2int(const char* str, int h = 0){
	    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

cmd_line::cmd_line(string a){
	this->cmd_input = a;
}

vector<string> cmd_line::split(string in, string delim){
	vector<string> result;
	if(strcmp(in.c_str(), "") == 0){
		return result;
	}
	char* in_char = new char[in.length()+1];
	char* delim_char = new char[delim.length()+1];
	strcpy(in_char, in.c_str());
	strcpy(delim_char, delim.c_str());

	char* s = strtok(in_char, delim_char);
	while(s){
		string temp = s;
		result.push_back(temp);
		s = strtok(NULL, delim_char);
	}
	delete []in_char;
	delete []delim_char;
	return result;

}

void cmd_line::set_input(string a){
	this->cmd_input = a;
}



void cmd_line::cmd_process(get_directory* dir){

	this->cmd_set = this->split(this->cmd_input, " ");

	dir->search_fileSet();
	vector<string> temp_fileSet = dir->get_fileSet();

	switch(str2int(this->cmd_set[0].c_str())){

		case str2int("ls"):{
			
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
				vector<string> dir_temp = this->split(dir->get_dir(), "/");
				dir_temp.pop_back();
				
				for(const auto a:dir_temp){
					new_dir += ("/" + a);
				}
				dir->set_dir(new_dir);
				break;
			}

			int s = 0;
			for(const auto a:temp_fileSet){
				if(!this->cmd_set[1].compare(a)){
					s = 1;
					break;
				}
			}
			if(!s){
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
			dir->search_fileSet();
			vector<string> temp_fileSet = dir->get_fileSet();
			int s = 0;
			for(const auto a:temp_fileSet){
				if(!this->cmd_set[1].compare(a)){
					s = 1;
					break;
				}
			}
			string new_path;
			if(!s){
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
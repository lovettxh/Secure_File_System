#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream> 
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "utilities.h"
#include "cmd_line.h"

using namespace std;


get_directory::get_directory(string a){
	this->current_dir = a + "/file";
	this->home_dir = a + "/file";
	if(opendir(this->home_dir.c_str()) == NULL){
		if(mkdir(this->home_dir.c_str(), 0777)){
			cout<<"Error: "<<strerror(errno)<<endl;
			exit(1);
		}
	}
}

void get_directory::set_dir(string a){
	this->current_dir = a;
}

void get_directory::set_user(string name){
	this->user = name;
	string dir = this->home_dir+"/"+name;
	if(opendir(dir.c_str()) == NULL){
		if(mkdir(dir.c_str(), 0777)){
			cout<<"Error: "<<strerror(errno)<<endl;
			exit(1);
		}
	}
}

string get_directory::get_dir(){
	return this->current_dir;
}

string get_directory::user_dir(){
	int l = this->home_dir.length();
	string t = this->current_dir.substr(l);
	t = "/home" + t;
	return t;
}

string get_directory::get_home_dir(){
	return this->home_dir;
}

void get_directory::search_fileSet(){
	this->file_set.clear();
	struct stat statBuffer;
	mode_t m;
	if(current_dir.empty()){
		cout<<"empty path"<<endl;
		return;
	}
	lstat(current_dir.c_str(), &statBuffer);
	m = statBuffer.st_mode;

	if(S_ISDIR(m)){
		struct dirent *dir;
		DIR *dp;
		string fileName;
		if((dp = opendir(current_dir.c_str())) == NULL){
			cerr<<"directory open error"<<endl;
		}
		while((dir = readdir(dp)) != NULL){
			if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
				continue;
			}
			fileName = current_dir + "/" + dir->d_name;
			lstat(fileName.c_str(), &statBuffer);
			if(S_ISREG(statBuffer.st_mode)){
				fileName = dir->d_name;
				this->file_set.push_back(fileName);
			}else if(S_ISDIR(statBuffer.st_mode)){
				fileName = "/";
				fileName += dir->d_name;
				this->file_set.push_back(fileName);
			}
		}
		closedir(dp);
	}
}

vector<string> get_directory::get_fileSet(){
	if(!this->current_dir.compare(this->home_dir)){
		vector<string> new_file_set;
		vector<string> user_set = get_group_member(this->user);
		for(auto a:this->file_set){
			for(auto b:user_set){
				if(!a.substr(1).compare(b)){
					new_file_set.push_back(a);
					break;
				}
			}
		}
		return new_file_set;
	}
	return this->file_set;
}	
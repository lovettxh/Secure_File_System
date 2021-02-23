#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "cmd_line.h"
using namespace std;


get_directory::get_directory(string a){
	this->current_dir = a;
	this->home_dir = a;
}

void get_directory::set_dir(string a){
	this->current_dir = a;
}

string get_directory::get_dir(){
	return this->current_dir;
}

void get_directory::set_home_dir(string a){
	this->home_dir = a;
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
	return this->file_set;
}

int main(){
	char buff[100];
	getcwd(buff, 100);
	get_directory dir(buff);
	cmd_line c("");

	string temp;

	while(1){
		cout<<dir.get_dir()<<"$ ";
		getline(cin, temp);
		
		c.set_input(temp);
		c.cmd_process(&dir);

	}
	return 0;
}
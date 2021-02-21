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

class get_directory{
public:
	get_directory(string a);
	void set_dir(string a);
	vector<string> get_fileSet();
	void search_fileSet();
private:
	string input_dir;
	vector<string> file_set;
};

get_directory::get_directory(string a){
	this->input_dir = a;
}

void get_directory::set_dir(string a){
	this->input_dir = a;
}

void get_directory::search_fileSet(){
	struct stat statBuffer;
	mode_t m;
	if(input_dir.empty()){
		cout<<"empty path"<<endl;
		return;
	}
	lstat(input_dir.c_str(), &statBuffer);
	m = statBuffer.st_mode;

	if(S_ISDIR(m)){
		struct dirent *dir;
		DIR *dp;
		string fileName;
		if((dp = opendir(input_dir.c_str())) == NULL){
			cerr<<"directory open error"<<endl;
		}
		while((dir = readdir(dp)) != NULL){
			if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
				continue;
			}
			fileName = input_dir + "/" + dir->d_name;
			lstat(fileName.c_str(), &statBuffer);
			if(S_ISREG(statBuffer.st_mode)){
				fileName = dir->d_name;
				this->file_set.push_back(fileName);
			}else if(S_ISDIR(statBuffer.st_mode)){
				fileName = "./";
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
	cmd_line c(buff);
	dir.search_fileSet();
	vector<string> t = dir.get_fileSet();
	string temp;
	for(const auto a : t){
		cout<<a<<endl;
	}
	while(1){
		
		getline(cin, temp);
		
		//c.set_input(temp);
		//c.cmd_process();

	}

	return 0;
}
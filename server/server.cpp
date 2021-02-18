#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

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
			cout<<"directory open error"<<endl;
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
	get_directory dir("/home/lovettxh/Secure_File_System/server");
	dir.search_fileSet();
	vector<string> t = dir.get_fileSet();
	//cout<<"1"<<endl;
	for(const auto a : t){
		cout<<a<<endl;
	}
	return 0;
}
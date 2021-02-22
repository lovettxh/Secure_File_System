#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector>
#include "cmd_line.h"
#include "server.h"
using namespace std;



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
	this->cmd_set = this->split(this->cmd_in, " ");

	switch(this->cmd_set[0]){
		case "ls":
			dir->search_fileSet();
			vector<string> temp = dir->get_fileSet();
			for(const auto a:temp){
				cout<<a<<"     ";
			}
			cout<<endl;
			break;

		case "cd":
			dir->search_fileSet();
			vector<string> temp = dir->get_fileSet();
			if(!this->cmd_set[1].compare("~")){
				dir->set_dir(dir->get_home_dir());
				break;
			}else if(!this->cmd_set[1].compare("..")){
				if(!dir->get_dir().compare(dir->get_home_dir())){
					break;
				}
				vector<string> dir_temp = this->split(dir->current_dir, "/");
				dir_temp.pop_back();
				string new_dir = "";
				for(const auto a:dir_temp){
					new_dir += ("/" + a);
				}
				dir->set_dir(new_dir);
				break;
			}

			int s = 0;
			for(const auto a:temp){
				if(!this->cmd_set[1].compare(a)){
					s = 1;
					break;
				}
			}
			if(s){
				string new_dir = dir->get_dir() + this->cmd_set[1];
				dir->set_dir(new_dir);
			}
			break;
	}
}
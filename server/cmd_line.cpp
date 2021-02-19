#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector>

class cmd_line{
public:
	void cmd_split();
	void set_input();
	void cmd_process();

private:
	string cmd_input;
	vector<string> cmd_set;
	vector<string> split(string in, string delim);
};

cmd_line::cmd_line(string a){
	this->cmd_input = a;
}

vector<string> cmd_line::split(string in, string delim){
	vector<string> result;
	if(strcmp(in, "") == 0){
		return result;
	}
	char* in_char = new char[in.length()+1];
	char* delim_char = new char[delim.length()+1];
	strcpy(in_char, in.c_str());
	strcpy(delim_char, delim.c_str());

	char* s = strtok(in_char, delim_char);
	while(s){
		string temp = p;
		result.push_back(temp);
		p = strtok(NULL, delim_char);
	}
	return result;

}

void cmd_line::set_input(string a){
	this->cmd_input = a;
}

void cmd_line::cmd_split(){
	
}
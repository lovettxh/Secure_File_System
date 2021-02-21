#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector>
#include "cmd_line.h"
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


void cmd_line::cmd_process(){
	this->cmd_set = this->split(this->cmd_input, " ");
	FILE *pp = popen(this->cmd_input.c_str(), "r");
	if(!pp){
		cerr<<"pipe open error"<<endl;
		return;
	}
	char out[1024];
	while(fgets(out, sizeof(out), pp)!=NULL){
		cout<<out<<endl;
	}
	pclose(pp);

}
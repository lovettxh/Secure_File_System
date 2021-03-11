#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <termios.h>
#include <sys/types.h>
#include "utilities.h"

using namespace std;


vector<string> split(string in, string delim){

    vector<string> res;
    if(in == "")
        return res;

    string str = in + delim;
    size_t pos = str.find(delim);

    while(pos != string::npos)
    {
        string temp = str.substr(0, pos);
        res.push_back(temp);

        str = str.substr(pos+delim.length(), str.size());
        pos = str.find(delim);
    }

    return res;
}

string string_replace(string input, string old_s, string new_s){
	size_t p = 0;
	while(input.find(old_s)!=string::npos){
		p = input.find(old_s);
		input.replace(p, old_s.length(), new_s);
	}
	return input;
}

string string_handle(string input){
	string new_s="";
	string temp;
	vector<string> str_set = split(input, "\\\\");
	for(const auto a:str_set){
		temp = a;
		temp = string_replace(temp,"\\n","\n");
		temp = string_replace(temp,"\\t","\t");
		temp+="\\";
		new_s+=temp;
	}
	new_s = new_s.substr(0, new_s.length()-1);
	return new_s;
}

// www.cplusplus.com/articles/E6vU7k9E/#UNIX-e1
int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

string str_length(string s){
    int a = s.length();
    string out = "";
    if(a<10){
        out = "00" + to_string(a);
    }else if(a>=10 && a<100){
        out = "0" + to_string(a);
    }else if(a>=100 && a<1000){
        out = to_string(a);
    }
    return out;
}

vector<string> get_group_member(string name){
    fstream file;
    file.open("l.txt", ios::in);
    string temp, group;
    vector<user> user_set;
    vector<string> user_info;
    vector<string> out;
    user u;
    while(getline(file, temp)){
        user_info = split(temp, " ");
        u = {user_info[0], user_info[1], user_info[2]};
        if(!user_info[0].compare(name)){
            group = user_info[2];
        }
        user_set.push_back(u);
    }
    for(auto a:user_set){
        if(!group.compare(a.group))
            out.push_back(a.id);
        
    }
    return out;
}


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
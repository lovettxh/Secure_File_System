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
#include <openssl/md5.h>
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

char *str2md5(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char* out = new char[33];

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}
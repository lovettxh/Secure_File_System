#include <string.h>
#include <ctype.h>
#include <cstring>
#include <vector>
using namespace std;

struct user{
	string id;
	string password;
	string group;
};
constexpr unsigned int str2int(const char* str, int h = 0){
	    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

vector<string> split(string in, string delim);
string string_handle(string input);
string string_replace(string input, string old_s, string new_s);
int getch();
string str_length(string s);
vector<string> get_group_member(string name);
int bsdChecksum(FILE *fp);

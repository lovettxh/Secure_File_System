#include <string.h>
#include <cstring>
#include <vector>
using namespace std;

class get_directory{
public:
	get_directory(string a);
	void set_dir(string a);
	void set_user(string name);
	vector<string> get_fileSet();
	void search_fileSet();
	string get_dir();
	string user_dir();
	string get_home_dir();
	char check_exist(string name);
	string convert_dir(string dir);
private:
	string current_dir;
	string home_dir;
	vector<string> file_set;
	string user;
};

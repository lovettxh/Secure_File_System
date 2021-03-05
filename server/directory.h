#include <string.h>
#include <cstring>
#include <vector>
using namespace std;

class get_directory{
public:
	get_directory(string a);
	void set_dir(string a);
	void set_home_dir(string a);
	vector<string> get_fileSet();
	void search_fileSet();
	string get_dir();
	string get_home_dir();
private:
	string current_dir;
	string home_dir;
	vector<string> file_set;
};

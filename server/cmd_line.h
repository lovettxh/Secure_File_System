#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "server.h"
using namespace std;

class cmd_line{
public:
	void set_input(string a);
	void directory_cmd(get_directory* dir);
	void file_cmd(get_directory* dir);
	cmd_line(string a);
private:
	string cmd_input;
	vector<string> cmd_set;
	void delete_dir(get_directory* dir, string dir_name);
	bool check_exist(get_directory* dir, string name);
};

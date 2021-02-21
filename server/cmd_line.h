#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
using namespace std;

class cmd_line{
public:
	void set_input(string a);
	void cmd_process();
	cmd_line(string a);
private:
	string cmd_input;
	vector<string> cmd_set;
	vector<string> split(string in, string delim);
};

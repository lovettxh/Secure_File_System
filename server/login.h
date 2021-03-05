#include <string>
#include <vector>
using namespace std;

struct user{
	string id;
	string password;
	string group;
};

class login{
public:
	void login_page();
	

private:
	void group_page();
	void read_group();
	void save_group();
	bool check_group(string name);
	bool user_page();
	bool user_sign_in();
	string password_input();
	bool check_user_name(string name);
	void read_user_set();
	void save_user_set();
	bool user_login();
	string password;
	vector<user> user_set;
	vector<string> group_set;
};


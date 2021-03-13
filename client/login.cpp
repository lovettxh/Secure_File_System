#include <iostream>
#include <fstream>
#include <termios.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "login.h"
#include "utilities.h"
using namespace std;


void login::set_fd(int fd){
	this->fd = fd;
}

void login::login_page(){
	char temp;
	cout<<"Welcome to SFS!"<<endl;
	bool c = false;
	while(!c){
		this->password = "";
		cout<<"Group/User:  [G/U]"<<endl;
		cin>>temp;
		if(temp == 'G' || temp == 'g'){
			this->group_page();
		}else if(temp == 'U' || temp == 'u'){
			c = this->user_page();
		}else{
			cout<<"Invalid input"<<endl;
		}
		write(this->fd, "0", 1);
		if(c){
			write(this->fd, "t", 1);
			write(this->fd, str_length(this->id).c_str(), 3);
			write(this->fd, this->id.c_str(), this->id.length());
		}else{
			write(this->fd, "f", 1);
		}
	}
}

void login::group_page(){
	char temp;
	string group_name;
	cout<<"Create group/View group:  [C/V]"<<endl;
	cin>>temp;
	if(temp == 'C' || temp == 'c'){
		cout<<"Please enter group name: ";
		cin>>group_name;
		this->read_group();
		if(this->check_group(group_name))
			cout<<"Group already exist"<<endl;
		else
			this->group_set.push_back(group_name);
		this->save_group();
	}else if(temp == 'V' || temp == 'v'){
		this->read_group();
		for(auto a:this->group_set)
			cout<<a<<"    ";
		cout<<endl;
	}else{
		cout<<"Invalid input"<<endl;
	}
}

void login::read_group(){
	char l[3];
	char g[1024];
	int s = 1;

	string group_name;
	this->group_set.clear();
	// string group_name;
	// fstream file;
	// file.open("g.txt",ios::in);
	// while(getline(file, group_name)){
	// 	this->group_set.push_back(group_name);
	// }
	// file.close();
	write(this->fd, "1", 1);
	while(s){
		char g[1024] = {0};
		read(this->fd, l, 3);
		s = atoi(l);
		read(this->fd, g, s);
		if(s){
			group_name = g;
			this->group_set.push_back(group_name);
		}
	}
}

void login::save_group(){
	// fstream file;
	// file.open("g.txt",ios::out);
	// for(auto a:this->group_set)
	// 	file<<a<<endl;
	// file.close();
	string temp;

	write(this->fd, "2", 1);
	for(auto a:this->group_set){
		temp = str_length(a);
		write(this->fd, temp.c_str(), 3);
		write(this->fd, a.c_str(), a.length());
	}
	write(this->fd, "000", 3);
}

bool login::check_group(string name){
	for(auto a:this->group_set){
		if(!name.compare(a)){
			return true;
		}
	}
	return false;
}

bool login::user_page(){
	char temp;
	cout<<"Sign in/Login:  [S/L]"<<endl;
	cin>>temp;
	if(temp == 'S' || temp == 's'){
		return this->user_sign_in();

	}else if(temp == 'L' || temp == 'l'){
		return this->user_login();
	}else{
		cout<<"Invalid input"<<endl;
	}
	return false;
}

bool login::user_sign_in(){
	cout<<"User name: ";
	string id;
	string p;
	string group;
	cin>>id;
	this->read_user_set();
	if(this->check_user_name(id)){
		cout<<"User name exist"<<endl;
		return false;
	}
	
	cout<<"Password: ";
	p = this->password_input();
	char* o = str2md5(p.c_str(), p.length());
	p = o;
	delete[] o;

	cout<<"Group: ";
	cin>>group;
	cin.ignore(10, '\n');
	this->read_group();
	if(!this->check_group(group)){
		cout<<"Group not exist"<<endl;
		return false;
	}
	user u = {id,p,group};
	this->user_set.push_back(u);
	this->save_user_set();
	return false;
}

string login::password_input(){
	cin.ignore(10, '\n');
	char temp;
	string p = "";
	int count = 0;
	while((temp = getch()) != 10){
		if((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')){
			count++;
			putchar('*');
			p += temp;
		}else if(temp == 127){
			if(count == 0){
				continue;
			}
			putchar('\b');
			putchar(' ');
			putchar('\b');
			count--;
			p = p.substr(0, p.length() - 1);
		}
	}
	cout<<endl;
	return p;
}

bool login::check_user_name(string name){
	this->password = "";
	for(auto a:this->user_set){
		if(!a.id.compare(name)){
			this->id = a.id;
			this->password = a.password;
			this->group = a.group;
			return true;
		}
	}
	return false;
}

void login::read_user_set(){
	char l[3];
	char u[1024];
	vector<string> user_info;
	string temp;

	int s = 1;
	user t;
	this->user_set.clear();
	write(this->fd, "3", 1);
	while(s){
		char u[1024] ={0};
		read(this->fd, l, 3);
		s = atoi(l);
		read(this->fd, u, s);
		if(s){
			temp = u;
			user_info = split(temp, " ");
			t = {user_info[0], user_info[1], user_info[2]};
			this->user_set.push_back(t);
		}

	}
	// fstream file;
	// file.open("l.txt", ios::in);
	// string temp;
	// user u;
	// vector<string> user_info;
	// this->user_set.clear();
	// while(getline(file, temp)){
	// 	user_info = split(temp, " ");
	// 	u = {user_info[0], user_info[1], user_info[2]};
	// 	this->user_set.push_back(u);
	// }
	// file.close();

}

void login::save_user_set(){
	string temp;

	write(this->fd, "4", 1);
	for(auto a:this->user_set){
		temp = a.id + " " + a.password + " " + a.group;
		write(this->fd, str_length(temp).c_str(), 3);
		write(this->fd, temp.c_str(), temp.length());
	}
	write(this->fd, "000", 3);
	// fstream file;
	// file.open("l.txt", ios::out);
	// string temp;

	// for(auto a:this->user_set){

	// 	temp = a.id + " " + a.password + " " + a.group;

	// 	file<<temp<<endl;
	// }
	// file.close();
}

bool login::user_login(){
	string id;
	string p;
	cout<<"User name: ";
	cin>>id;
	cout<<"Password: ";
	p = this->password_input();
	char* o = str2md5(p.c_str(), p.length());
	p = o;
	delete[] o;
	this->read_user_set();
	if(!this->check_user_name(id) || p.compare(this->password)){
		cout<<"Wrong password"<<endl;
		return false;
	}
	return true;
}


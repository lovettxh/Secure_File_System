#include <iostream>
#include <fstream>
#include <termios.h>
#include <stdio.h>
#include <string>
#include "login.h"
#include "utilities.h"
using namespace std;

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
	this->group_set.clear();
	string group_name;
	fstream file;
	file.open("g.txt",ios::in);
	while(getline(file, group_name)){
		this->group_set.push_back(group_name);
	}
	file.close();
}

void login::save_group(){
	fstream file;
	file.open("g.txt",ios::out);
	for(auto a:this->group_set)
		file<<a<<endl;
	file.close();
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
	string password;
	string group;
	cin>>id;
	this->read_user_set();
	if(this->check_user_name(id)){
		cout<<"User name exist"<<endl;
		return false;
	}
	
	cout<<"Password: ";
	password = this->password_input();

	cout<<"Group: ";
	cin>>group;
	this->read_group();
	if(!this->check_group(group)){
		cout<<"Group not exist"<<endl;
		return false;
	}
	user u = {id,password,group};
	this->user_set.push_back(u);
	this->save_user_set();
	return true;
}

string login::password_input(){
	cin.ignore(10, '\n');
	char temp;
	string password = "";
	int count = 0;
	while((temp = getch()) != 10){
		if((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')){
			count++;
			putchar('*');
			password += temp;
		}else if(temp == 127){
			if(count == 0){
				continue;
			}
			putchar('\b');
			putchar(' ');
			putchar('\b');
			count--;
			password[count] = NULL;
		}
	}
	cout<<endl;
	return password;
}

bool login::check_user_name(string name){
	this->password = "";
	for(auto a:this->user_set){
		if(!a.id.compare(name)){
			this->password = a.password;
			return true;
		}
	}
	return false;
}

void login::read_user_set(){
	fstream file;
	file.open("l.txt", ios::in);
	string temp;
	user u;
	vector<string> user_info;
	this->user_set.clear();
	while(getline(file, temp)){
		user_info = split(temp, " ");
		u = {user_info[0], user_info[1], user_info[2]};
		this->user_set.push_back(u);
	}
	file.close();
}

void login::save_user_set(){
	fstream file;
	file.open("l.txt", ios::out);
	string temp;

	for(auto a:this->user_set){

		temp = a.id + " " + a.password + " " + a.group;

		file<<temp<<endl;
	}
	file.close();
}

bool login::user_login(){
	string id;
	string password;
	cout<<"User name: ";
	cin>>id;
	cout<<"Password: ";
	password = this->password_input();

	this->read_user_set();
	if(!this->check_user_name(id) || password.compare(this->password)){
		cout<<"Wrong password"<<endl;
		return false;
	}
	return true;
}


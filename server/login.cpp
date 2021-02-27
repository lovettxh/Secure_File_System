#include <iostream>
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
		this->id = "";
		this->password = "";
		cout<<"Sign in/Login:  [S/L]"<<endl;
		cin>>temp;
		if(temp == 'S' | temp == 's'){
			c = this->sign_in();

		}else if(temp == 'L' | temp == 'l'){
			c = this->login_login();
		}else{
			cout<<"Invalid input"<<endl;
		}
	}
	cout<<this->id<<endl;
	cout<<this->password<<endl;
}

bool login::sign_in(){
	cout<<"User name: ";;
	string t;
	cin>>this->id;
	cin.ignore(10, '\n');
	cout<<"Password: ";
	char temp;
	int count = 0;
	while((temp = getch()) != 10){
		if((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9')){
			count++;
			putchar('*');
			this->password += temp;
		}else if(temp == 127){
			if(count == 0){
				continue;
			}
			putchar('\b');
			putchar(' ');
			putchar('\b');
			count--;
			this->password[count] = NULL;
		}
	}
	return true;
}

bool login::login_login(){
	return true;
}
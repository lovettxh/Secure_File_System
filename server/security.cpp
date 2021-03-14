#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include "security.h"

using namespace std;

// string encrypt function
string dir_encrypt(string c) {
	int key[] = {1,2,3,4,5,6,7}; 
	string out = c;
	int len = c.size();
	for (int i = 0; i < len; i++) {
		out[i] = out[i] ^ key[i % 7];
	}
	return out;
}

// string decrypt function
string dir_decode(string c) {
	int key[] = {1,2,3,4,5,6,7}; 
	string out = c;
	int len = c.size();
	for (int i = 0; i < len; i++) {
		out[i] = out[i] ^ key[i % 7];
	}
	return out;
}

// calculate a file's bsdChecksum and save it for integrity check
void integrity_save(get_directory* dir, string save_dir, string current_dir){
	fstream save_file;
	FILE* pfile;
	string file_path;
	dir->set_dir(current_dir);
	dir->search_fileSet();
	vector<string> file_set = dir->get_fileSet();
	vector<string> new_file_set;
	int cs;
	save_file.open(save_dir.c_str(), ios::out | ios::app);
	for(auto a:file_set){
		if(!a.compare("integ.dat")){
			continue;
		}
		if(a[0] == '/'){
			file_path = current_dir + a;
			save_file<<file_path<<" 0"<<endl;
			new_file_set.push_back(a);
		}else{
			file_path = current_dir + "/" + a;
			pfile = fopen(file_path.c_str(), "rb");
			cs = bsdChecksum(pfile);
			fclose(pfile);
			save_file<<file_path<<" "<<to_string(cs)<<endl;
		}
	}
	save_file.close();
	for(auto a:new_file_set){
		file_path = current_dir + a;
		integrity_save(dir, save_dir, file_path);
	}
}

// check every file's bsdChecksum under user directory to 
// find out whether a file has been changed 
void check_file_integrity(get_directory* dir, int fd){
	fstream f;
	string temp;
	string message;
	vector<string> data, temp_path;
	FILE* pfile;
	string old_dir = dir->get_dir();
	string save_dir = dir->get_home_dir() + "/.integ/integ." + dir->get_user() + ".dat";
	f.open(save_dir.c_str(), ios::in);
	int s = 1;
	while(getline(f, temp)){
		data = split(temp, " ");
		if((pfile = fopen(data[0].c_str(), "rb")) == NULL || bsdChecksum(pfile) != stoi(data[1])){
		
			temp = data[0].substr(dir->get_home_dir().length());
			temp_path = split(temp,"/");
			message = "File: /home";
			for(int i = 0; i < temp_path.size() - 1; i++){
				if(temp_path[i].compare("")){
					message += ("/" + temp_path[i]);
				}
			}
			message += ("/" + dir_decode(temp_path.back()) + " is damaged\n");
			write(fd,"o",1);
			write(fd, str_length(message).c_str(), str_length(message).length());
			write(fd, message.c_str(), message.length());
			s = 0;
		}
		fclose(pfile);
	}
	f.close();
	if(s){
		message = "File integrity check passed\n";
		write(fd,"o",1);
		write(fd, str_length(message).c_str(), str_length(message).length());
		write(fd, message.c_str(), message.length());
	}
	return;
}

// save every file's bsdChecksum value under user directory for integrity check
void save_file_integrity(get_directory* dir){
	fstream f;
	vector<string> file_set;
	string old_dir = dir->get_dir();
	string current_dir = dir->get_home_dir() + "/.integ";
	if(opendir(current_dir.c_str()) == NULL){
		if(mkdir(current_dir.c_str(), 0777)){
			cout<<"Error: "<<strerror(errno)<<endl;
			exit(1);
		}
	}
	string save_dir = dir->get_home_dir() + "/.integ/integ." + dir->get_user() + ".dat";
	f.open(save_dir.c_str(), ios::out | ios::trunc);
	f.close();
	current_dir = dir->get_home_dir() + "/" + dir->get_user();
	integrity_save(dir, save_dir, current_dir);
	dir->set_dir(old_dir);

}

// encrypt the file given its name, output file name and key
void file_encrypt(string file_path, string file_name ,string k){

	unsigned char key[EVP_MAX_KEY_LENGTH], in[1024], out[1024];
	strncpy((char*)key, k.c_str(), EVP_MAX_KEY_LENGTH);
	unsigned char iv[EVP_MAX_KEY_LENGTH] = {0};
	string inp = file_path + "/" + file_name;
	int in_length, out_length;
	EVP_CIPHER_CTX *ctx;
	FILE* fin;
	FILE* fout;
	if((fin = fopen(inp.c_str(), "rb")) == NULL){
		cout<<"Can't open source file"<<endl;
		return;
	}
	ctx = EVP_CIPHER_CTX_new();
	if(EVP_EncryptInit_ex(ctx,EVP_aes_256_ecb(),NULL,key,iv) != 1){
		cout<<"EVP_EncryptInit Error"<<endl;
		return;
	}
	string out_file = dir_encrypt(file_name);
	string outp = file_path + "/" + out_file;

	if((fout = fopen(outp.c_str(), "wb")) == NULL){
		cout<<"Can't open target file"<<endl;
		return;
	}
	while(true){
		in_length = fread(in, 1, 1024, fin);
		if(in_length <= 0){
			break;
		}
		if(EVP_EncryptUpdate(ctx,out,&out_length,in,in_length) != 1){
			fclose(fin);
			fclose(fout);
			cout<<"EVP_Encrypt Error"<<endl;
			return;
		}
		fwrite(out, 1, out_length, fout);
	}
	EVP_EncryptFinal_ex(ctx,out,&out_length);
	fwrite(out, 1, out_length, fout);
	fclose(fin);
	fclose(fout);
	EVP_CIPHER_CTX_cleanup(ctx);
	remove(inp.c_str());
	return;
}

// decrypt the file given its name, output file name and key
void file_decrypt(string file_path, string file_name, string k){

	unsigned char key[EVP_MAX_KEY_LENGTH], in[1024], out[1024 + EVP_MAX_KEY_LENGTH];
	strncpy((char*)key, k.c_str(), EVP_MAX_KEY_LENGTH);
	unsigned char iv[EVP_MAX_KEY_LENGTH]= {0} ;
	int in_length, out_length;
	string inp = file_path + "/" + file_name;
	EVP_CIPHER_CTX *ctx;
	FILE* fin;
	FILE* fout;

	if((fin = fopen(inp.c_str(), "rb")) == NULL){
		cout<<"Can't open source file"<<endl;
		return;
	}
	ctx = EVP_CIPHER_CTX_new();
	if(EVP_DecryptInit_ex(ctx,EVP_aes_256_ecb(),NULL,key,iv) != 1){
		EVP_CIPHER_CTX_cleanup(ctx);
		cout<<"EVP_DecryptInit Error"<<endl;
		return;
	}
	string out_file = dir_decode(file_name);
	string outp = file_path + "/" + out_file;
	if((fout = fopen(outp.c_str(), "wb")) == NULL){
		cout<<"Can't open target file"<<endl;
		return;
	}
	while(true){
		in_length = fread(in, 1, 1024, fin);
		if(in_length <= 0){
			break;
		}
		if(EVP_DecryptUpdate(ctx,out,&out_length,in,in_length) != 1){
			fclose(fin);
			fclose(fout);
			cout<<"EVP_Encrypt Error"<<endl;
			return;
		}
		fwrite(out,1,out_length,fout);
	}
	EVP_DecryptFinal_ex(ctx,out,&out_length);
	fwrite(out, 1, out_length, fout);
	fclose(fin);
	fclose(fout);
	EVP_CIPHER_CTX_cleanup(ctx);
	remove(inp.c_str());
	return;
}

// encrypt all the file content, file name and directory name under user directory
void user_encrypt(get_directory* dir, string path){
	string old_dir = dir->get_dir();
	dir->set_dir(path);
	dir->search_fileSet();
	string new_path;
	vector<string> temp = dir->get_fileSet();
	vector<string> dir_set, file_set;
	for(auto a:temp){
		if(a[0] == '/'){
			dir_set.push_back(a);
		}else{
			file_set.push_back(a);
		}
	}
	for(auto a:dir_set){
		new_path = path + a;
		user_encrypt(dir, new_path);
		rename((path + a).c_str(), (path + "/" + dir_encrypt(a.substr(1))).c_str());
	}
	for(auto a:file_set){
		file_encrypt(path, a, dir->get_user());
	}
	dir->set_dir(old_dir);
}

// decrypt all the file content, file name and directory name under user directory
void user_decrypt(get_directory* dir, string path){
	string old_dir = dir->get_dir();
	dir->set_dir(path);
	dir->search_fileSet();
	string new_path;
	vector<string> temp = dir->get_fileSet();
	vector<string> dir_set, file_set;
	for(auto a:temp){
		if(a[0] == '/'){
			dir_set.push_back(a);
		}else{
			file_set.push_back(a);
		}
	}
	for(auto a:dir_set){
		new_path = path + a;
		user_decrypt(dir, new_path);
		rename((path + a).c_str(), (path + "/" + dir_decode(a.substr(1))).c_str());
	}
	for(auto a:file_set){
		file_decrypt(path, a, dir->get_user());
	}
	dir->set_dir(old_dir);
}
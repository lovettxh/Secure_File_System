#include <string>

#include "cmd_line.h"

using namespace std;
void check_file_integrity(get_directory* dir, int fd);
void save_file_integrity(get_directory* dir);
void integrity_interate(get_directory* dir, string save_dir, string current_dir);
void file_encrypt(string file_path, string file_name ,string k);
void file_decrypt(string file_path, string file_name ,string k);
void user_encrypt(get_directory* dir, string path);
void user_decrypt(get_directory* dir, string path);

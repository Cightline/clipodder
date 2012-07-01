
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#ifndef FILESYSTEM
#define FILESYSTEM

namespace filesystem 
{
  bool file_exists(std::string path);
  bool is_dir(std::string path);
  bool make_dir(std::string path);  
  int  list_dir(std::string path, std::vector<std::string> &file_vector);
  int  get_mtime(std::string file_path);
  int  remove_file(std::string file_path);
  std::string get_home();

};
#endif

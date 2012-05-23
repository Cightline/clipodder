
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <string>

#ifndef FILESYSTEM
#define FILESYSTEM
class filesystem
{

public: 
  bool file_exists(std::string path);
  
  bool is_dir(std::string path);
  
  bool make_dir(std::string path);
  
  std::vector<std::string> list_dir(std::string path);

};
#endif


#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>


class filesystem
{

public: 
  bool file_exists(std::string path);
  
  bool is_dir(std::string path);


};

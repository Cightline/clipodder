
#include <iostream>
#include <vector>
#include <string> 


#include "filesystem.hpp"
#include "debug.hpp"

namespace file_manager 
{
  int delete_excess(std::string path, int max_files);
  int prepare_download(std::string parent_dir, std::string final_dir);
  std::string get_final_dir(std::string title, std::string parent_dir);
};

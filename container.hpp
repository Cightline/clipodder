
#include <map>
#include <vector>
#include <string>

#ifndef CONTAINER
#define CONTAINER
class container
{
public:
  int max_downloads;
  int no_child_dir;
  int num_formats;
  int num_downloads; 
  int save_as_title;
  
  std::string download_dir;
  std::string final_dir;
  std::string add_extension;
  std::string command;

  std::string title;
  std::string item_title;
  std::string url;
  std::string *data;
  std::string dir_name;
  
  std::vector<std::string> config_formats;
  std::vector<std::string> link_vector;
  std::map<std::string, std::string> format_map;
 

};
#endif

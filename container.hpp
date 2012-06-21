
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
  
  std::string download_dir;
  std::string final_dir;

  std::string *title;
  std::string *url;
  std::string *data;
  std::string dir_name;
  
  std::vector<std::string> config_formats;
  std::vector<std::string> link_vector;
  std::map<std::string, std::string> format_map;

  int set_title(std::string title);
  int set_url(std::string url);
  int set_final_dir(std::string final_dir);
  std::string get_final_dir();
  int done();
};
#endif

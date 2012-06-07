#include <string>
#include <vector>
#include <iostream>
#include <map>

class container
{
public:
  
  std::map<std::string, std::string> media_urls;
  std::vector<std::string> format_vector;
  std::string title;
  std::string save_dir;
  std::string *data;
  std::string url;

  int set_url(std::string url);
  int add_media_url(std::string url, std::string format);
  int add_format(std::string format);
  int set_title(std::string title);
  int set_save_dir(std::string dir);
  int set_data(std::string *data);

};



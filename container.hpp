#include <string>
#include <vector>
#include <iostream>
#include <map>

class container
{
public:
  
  std::map<std::string, std::string> media_urls;
  std::vector<std::string> format_vector;
  std::string *title;
  std::string save_dir;
  std::string *data;
  std::string *url;

  container();
  int set_data(std::string *data);
  int set_url(std::string url);
  int set_title(std::string title);


};



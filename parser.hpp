

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <cstring>

#include "network.hpp"
#include "output.hpp"



class parser
{

private:
  std::string *url;
  std::string *data;

public:
  void done();

  ~parser();
  
  int set_url(std::string url);
  int set_data(std::string *data);
  int get_links();
  
  std::vector<std::string> link_vector;
  std::map<std::string, std::string> format_map;
  std::string get_title();
  std::string get_item_title();
  int parse_feed();
};


#include <confuse.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "debug.hpp"


class config

{
public:
  
  static std::map<std::string, std::vector<std::string> > url_map;
  static std::map<std::string, int> max_downloads_map;
  static std::map<std::string, std::string> config_map;
  static std::map<std::string, std::string> download_map;
  
  int parse_config();
  std::string get_home(); 
};



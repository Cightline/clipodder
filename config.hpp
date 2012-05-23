
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
  debug dbg;

public:
  
  
  static std::map<std::string, std::vector<std::string> > url_map;
  static std::map<std::string, int> max_downloads_map;
  
  static std::map<std::string, std::string> config_map;
  
  
  int parse_config();


  std::map<std::string, std::vector<std::string> > current_urls();  
  std::string get_home();
  
  

};



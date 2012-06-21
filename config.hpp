
#include <confuse.h>
#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "debug.hpp"
#include "container.hpp"

class config

{
public:
  
  std::map<std::string, std::vector<std::string> > url_map;
  std::map<std::string, int> max_downloads_map;
  std::map<std::string, std::string> config_map;
  std::map<std::string, std::string> download_map;
  std::map<std::string, std::string> dir_name_map;
  std::map<std::string, int> no_child_map;
  
  std::vector<container> parse_config();
  std::string get_home(); 
};



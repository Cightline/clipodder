
#include <confuse.h>
#include <iostream>
#include <map>
#include <vector>

#include "debug.hpp"
#include "container.hpp"
#include "filesystem.hpp"
#include "global_config.hpp"


#ifndef CONFIG_HPP
#define CONFIG_HPP
class config
{

private:
  std::map<std::string, std::string> config_map;

public:
  std::vector<container> parse_config();
  
  static std::map<std::string, std::string> global_config;

};
#endif


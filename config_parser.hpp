
#include <confuse.h>
#include <iostream>
#include <map>
#include <vector>

#include "debug.hpp"
#include "container.hpp"
#include "filesystem.hpp"
#include "global_config.hpp"
#include "utils.hpp"
#include "output.hpp"


#ifndef CONFIG_HPP
#define CONFIG_HPP
class config_parser
{

private:
  std::map<std::string, std::string> config_map;
  std::vector<container> all_podcasts;
  cfg_t *cfg;

public:
  int parse_config(std::string config_path);
  std::vector<container> get_podcasts();
  std::string get_value(std::string key);
  void done();

  
};
#endif


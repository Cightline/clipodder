
/* Class to parse the configuration files */

#include <map>
#include <vector>
#include <string> 

#include <iostream>
#include <fstream>

#include <confuse.h>


#include "network.hpp"
#include "format.hpp"
#include "parser.hpp"
#include "filesystem.hpp"
#include "file_manager.hpp"
#include "container.hpp"
#include "global_config.hpp"
#include "utils.hpp"
#include "output.hpp"


class core
{

public:

  
  int download_podcasts(container &podcast);
  int download_link(std::string url, std::string title, std::string final_dir);
  int determine_download_dir(container &podcast);

  
  std::map<std::string, std::string> path_map; 

};


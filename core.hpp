
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

#include "debug.hpp"


class core
{

public:

  struct data_
  {
    std::string *title;
    std::string *data;
    std::string *url;
  };
  
  
  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  int parse_given_format(std::string to_parse, std::string *format, std::string *extension);

  bool defined_type(std::vector<std::string> f_vector, std::string extension, std::string format);
  
  int download_podcasts(container &podcast);


  int download_link(std::string url, std::string title, std::string final_dir);

  
  std::map<std::string, std::string> path_map; 

};


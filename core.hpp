
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
  
  int done();
  
  bool should_download(std::string url, 
		       std::string media_url, 
		       std::string supplied_info, //rename to supplied_format
		       std::vector<std::string> format_vector);


  int save_download_path(std::string address, std::string path);
  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  int parse_given_format(std::string to_parse, std::string *format, std::string *extension);

  bool defined_type(std::vector<std::string> f_vector, std::string extension, std::string format);
  
  int download_podcasts(std::string url, 
			int max_downloads, 
			std::string download_dir, 
			std::vector<std::string> formats);


  int download_link(std::string url, std::string title, std::string final_dir);

  
  std::map<std::string, std::string> path_map; 

};


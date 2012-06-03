
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
#include "downloader.hpp"


#include "podcast_container.hpp"
#include "debug.hpp"


class core
{

public:

  parser ps;
    
  bool should_download(std::string url, 
		       std::string media_url, 
		       std::string supplied_info,
		       std::vector<std::string> format_vector);



  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  int fill_container(podcast_container *container);
  int parse_given_format(std::string to_parse, std::string *format, std::string *extension);

  bool defined_type(std::vector<std::string> f_vector, std::string extension, std::string format);
  
  std::string fetch_page(std::string url);

  int download_podcasts(std::string url, 
			int max_downloads, 
			std::string download_dir, 
			std::vector<std::string> formats);


  int deal_with_link(std::string url, std::string title, std::string *final_dir);
  int get_filename(std::string url, std::string *return_url);

  int prepare_download(std::string url, std::string title, std::string *final_dir);

  std::string get_podcast_url(std::string format);
  void setup_formats();
  int known_formats(std::string format);
  int parse_buffer(const char *buffer, size_t size, const char *url);
  int delete_uneeded(std::string path, int max_downloads);

  std::vector<std::string> links;
  
  std::string save_dir; 

};


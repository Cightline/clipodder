
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
#include "config.hpp"
#include "filesystem.hpp"
#include "downloader.hpp"


#include "podcast_container.hpp"


static int curl_write(void *buffer, size_t size, size_t nmemb, void *data);



class core
{

public:

  // I like to re-use these. 
  config cfg;
  network net;
  filesystem fs;
  downloader dl;
  parser ps;

  bool should_download(std::string url, std::string media_url);
  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  int fill_container(podcast_container *container);
  int parse_config();
  int validate_urls(std::map<std::string, std::vector<std::string> > urls);
  
  std::string fetch_page(std::string url);
  int download_podcasts(std::map<std::string, std::vector<std::string> > urls);
  int deal_with_link(std::string url, std::string title);
  int get_filename(std::string url, std::string *return_url);

  std::string get_podcast_url(std::string format);
  void setup_formats();
  int known_formats(std::string format);
  int parse_buffer(const char *buffer, size_t size, const char *url);

  
  std::map<std::string, std::vector<std::string> > urls; 
  std::map<std::string, std::vector<std::string> >::iterator u_iter;

  std::vector<std::string> links;
  
  std::string save_dir; 

};


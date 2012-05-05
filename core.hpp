
/* Class to parse the configuration files */

#include <map>
#include <vector>
#include <string> 

#include <iostream>
#include <fstream>

#include <confuse.h>


static int curl_write(void *buffer, size_t size, size_t nmemb, void *data);



class core
{

public:
  int init();
  
  int parse_config();
  std::string fetch_page(std::string url);
  int download_podcasts();
  int download_file(std::string url, std::string title);
  int get_filename(std::string url, std::string *return_url);

  std::string get_podcast_url(std::string format);
  void setup_formats();
  int known_formats(std::string format);
  int parse_buffer(const char *buffer, size_t size, const char *url);

  
  std::map<std::string, std::vector<std::string> > url_map;
  std::vector<std::string> k_formats;

  std::map<std::string, std::vector<std::string> > urls; 
  std::map<std::string, std::vector<std::string> >::iterator u_iter;

  std::vector<std::string> links;
  
  std::string save_dir; 

};


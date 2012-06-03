#include <vector>
#include <string>

#include "debug.hpp"
#include "filesystem.hpp"

#ifndef DOWNLOADER
#define DOWNLOADER

namespace downloader
{
  
  int prepare_download(std::string title, 
		       std::string media_url, 
		       std::string parent_dir, 
		       std::string *final_dir);
};

#endif

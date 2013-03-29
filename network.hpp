
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


#include <curl/curl.h>

#include "debug.hpp"
#include "output.hpp"

static int curl_write(void *buffer, size_t size, size_t nmemb, void *data);
static int curl_write_file(void *buffer, size_t size, size_t nmemb, void *data);
static int progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

namespace network 
{
  std::string *fetch_page(std::string url);
  int download_file(std::string url, std::string download_path, bool show_progress);
};

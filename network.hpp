
#include <curl/curl.h>
#include <string>
#include <fstream>




static int curl_write(void *buffer, size_t size, size_t nmemb, void *data);
static int curl_write_file(void *buffer, size_t size, size_t nmemb, void *data);

class network
{

public:
  std::string fetch_page(std::string url);
  
  int download_file(std::string url, std::string download_path);
};


#include <curl/curl.h>
#include <string>




static int curl_write(void *buffer, size_t size, size_t nmemb, void *data);

class network
{

public:
  std::string fetch_page(std::string url);

};

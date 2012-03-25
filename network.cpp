
/* Mostly libcurl stuff */

//#include "clipodder.hpp"

#include "network.hpp"

static int curl_write(void *buffer, size_t size, size_t nmemb, void *data)
{
  //https://github.com/akrennmair/newsbeuter/blob/master/rss/parser.cpp                                                
  /* cast to std::string */
  std::string *pbuf = static_cast<std::string *>(data);

  pbuf->append(static_cast<const char *>(buffer), size *nmemb);
  return size * nmemb;


}

std::string network::fetch_page(std::string url)
{
 

  std::string buf = NULL;
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
 
  if(curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
      res = curl_easy_perform(curl);

      curl_easy_cleanup(curl);
    }
  return buf;
}


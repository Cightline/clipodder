
#include <vector>
#include <map>

#ifndef PODCAST_HEADER
#define PODCAST_HEADER
struct podcast_container
{
  std::string url;
  std::string data;
  std::string title;
  std::vector<std::string> links;
  std::map<std::string, std::string> media_urls;
};


#endif

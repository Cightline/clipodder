
#include <vector>
#include <map>

#ifndef PODCAST_HEADER
#define PODCAST_HEADER
struct podcast_container
{
  std::string url;
  std::string data;
  std::string title;
  int max_downloads;
  std::vector<std::string> links;
  std::map<std::string, std::string> formats;
};


#endif

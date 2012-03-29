
#include <confuse.h>
#include <iostream>
#include <map>
#include <vector>

class config 
{
public:
  std::map<std::string, std::vector<std::string> > url_map;

  int parse_config();
  std::map<std::string, std::vector<std::string> > current_urls();


};

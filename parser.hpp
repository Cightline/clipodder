
#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>


#include <cstring>



class parser
{
public:
  xmlDoc *doc;
  std::map<std::string, std::vector<std::string> > url_map;
  int parse_buffer(const char *buffer, size_t size, const char *url);
  std::string get_content(xmlNode *node);
  std::vector<std::string> get_links(xmlNode *node);
  int parse_feed(xmlNode *root_node);
};

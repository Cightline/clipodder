
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
  std::vector<std::string> link_vector;
  std::map<std::string, std::vector<std::string> > url_map;
  xmlNode *  parse_buffer(const char *buffer, size_t size, const char *url);
  std::string get_content(xmlNode *node);
  std::vector<std::string> get_links(xmlNode *node);
  int parse_feed(std::string page, std::string url);
  std::string get_attr(xmlNode *node, const char *attr);
  bool node_is(xmlNode *node, const char *name);
  std::vector<xmlNode *> node_vector(xmlNode *node, const char *name);
  std::vector<std::string> return_links();


  
};

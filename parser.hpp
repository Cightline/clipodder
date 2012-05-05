
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
  xmlNode *g_root_node;

  std::vector<std::string> link_vector;

  int reset();

  bool root_node_exists();

  
  std::map<std::string, std::vector<std::string> > url_map;
  
  xmlNode *  parse_buffer(const char *buffer, size_t size, const char *url);
  
  std::string get_content(xmlNode *node);
  std::string get_title();

  
  int get_all_links(std::vector<std::string> *link_vector);
  int get_links_from_node(xmlNode *node, std::vector<std::string> *link_vector);
  int parse_links(xmlNode *node);


  int parse_feed(std::string page, std::string url);
  
  std::string get_attr(xmlNode *node, const char *attr);
  
  bool node_is(xmlNode *node, const char *name);
  
  std::vector<xmlNode *> node_vector(xmlNode *node, const char *name);
  
  std::vector<std::string> return_links();


  
};

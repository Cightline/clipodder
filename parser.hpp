
#include <string>
#include <iostream>
#include <map>
#include <vector>


#include <libxml/parser.h>
#include <libxml/tree.h>


#include <cstring>

#include "debug.hpp"
#include "network.hpp"

class parser
{

public:

  struct podcast_container
  {
    std::string url;
    std::string title;
    std::vector<std::string> links;
    std::map<std::string, std::string> media_urls;
  };
  
  xmlDoc *doc;
  xmlNode *root_node;
  std::string *url;
  std::string *data; 

  bool root_node_exists();

  int set_url(std::string *url);
  int set_data(std::string *data);
  int done();

  int get_links();

  int get_enclosures(std::vector<xmlNode *> *vect);
  xmlNode *get_node(xmlNode *node_with_children, std::string name);


  std::map<std::string, std::vector<std::string> > url_map;
  
  xmlNode* parse_buffer(const char *buffer, size_t size, const char *url);
  
  std::string get_content(xmlNode *node);
  std::string get_title();
  
  int parse_feed();
  
  std::string get_attr(xmlNode *node, const char *attr);
  
  bool node_is(xmlNode *node, const char *name);
  
  std::vector<xmlNode *> node_vector(xmlNode *node, const char *name);

  std::vector<std::string> link_vector;
  std::map<std::string, std::string> format_map;


  
};

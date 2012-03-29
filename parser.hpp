
#include <string>
#include <iostream>
#include <map>


#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstring>



class parser
{
public:
  int parse_buffer(const char *buffer, size_t size, const char *url);
  int store_xml_nodes(xmlDoc *xml_page, const char *url);
 
  int parse_xml(xmlDoc *doc);
 
  
};

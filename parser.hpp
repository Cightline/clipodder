
#include <string>
#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>




class parser
{
public:
  int parse_buffer(const char *buffer, size_t size, const char *url);


};

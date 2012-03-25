


#include "parser.hpp"


int parser::parse_buffer(const char *buffer, size_t size, const char *url)
{
  /* Parse whats currently in the buffer */

  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;


  doc = xmlReadMemory(buffer, size, url, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

  if (doc == NULL)
    {
      std::cout << "Could not parse xml" << std::endl;
      return 1;
    }

  root_element = xmlDocGetRootElement(doc);

  //if (root_element->type == XML_ELEMENT_NODE)                                                                        

  std::cout << "Node: " << root_element->name << std::endl;




  xmlFreeDoc(doc);

}

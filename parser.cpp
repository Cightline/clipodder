
#include "parser.hpp"

std::string get_node_content(xmlNode *node)
{
  std::string return_value;
  
  if (node)
    {
      xmlChar *content = xmlNodeGetContent(node);
      
      if (content)
	{
	  return_value = (const char *)content;
	  // xmlFree(node);
	}
    }
  return return_value;
}


std::string get_link(xmlNode *cur)
{
  xmlChar *uri;
  //cur = cur->xmlChildrenNode;
  
  while (cur != NULL)
    {
      std::cout << cur->name << std::endl;
      

      //std::cout << get_node_content(cur) << std::endl;
      cur = cur->next; 
    }
     
  
  return "what";

}

std::string get_prop(xmlNode *node, const char *prop)
{
  std::string return_value;
  
  if (node)
    {
      xmlChar *value;
      
      value = xmlGetProp(node, (xmlChar *)prop);
      
      if (value)
	{
	  return_value = (const char *)value;
	  
	}
    }
  return return_value;
}


int parser::parse_xml(xmlDoc *doc)
{

  xmlNode* root_element = NULL;
  xmlNode* cur_node = NULL;

  
  root_element = xmlDocGetRootElement(doc);

   
  for (cur_node = root_element->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
        {
	  std::cout << "Name: " << cur_node->name << std::endl;
	  
	  get_link(cur_node->children);

	  
	  
	}
      
      

    }
  
  xmlFreeDoc(doc);
  
}  



int parser::parse_buffer(const char *buffer, size_t size, const char *url)
{
  
  
  xmlDoc *doc = NULL;
  xmlNode* root_element = NULL;
  xmlNode* cur_node = NULL;

  doc = xmlReadMemory(buffer, size, url, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

  if (doc == NULL)
    {
      std::cout << "Could not parse xml" << std::endl;
      return 0;
    }
  
  else
    {
      parser::parse_xml(doc);
    }
  
}



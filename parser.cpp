
#include "parser.hpp"



std::string get_prop(xmlNode *node, const char *prop)
{
  std::string return_s;
  if (node)
    {
      xmlChar *value;
      
      value = xmlGetProp(node, (xmlChar *)prop);
      
      if (value)
	{
	  return_s = (const char *)value;
	  xmlFree(value);
	}

     
    }

  return return_s;
}

bool node_is(xmlNode *node, const char * name)
{
  if (strcmp((const char *)node->name, name) == 0)
    {
      std::cout << "Node is " << name << std::endl;
      return true;
    }

  return false;
}


std::vector<xmlNode *> node_vector(xmlNode *node, const char *name)
{
  std::vector<xmlNode *> return_vector;

  for (xmlNode *t_node = node->children; t_node != NULL; t_node = t_node->next)
    {
      if (node_is(t_node, name))
	{
	  return_vector.push_back(t_node);
	}
    }
  

  return return_vector;

}


std::vector<std::string> parser::get_links(xmlNode *node)
{
  /* I tried to make this as pretty as possible, and it still looks pretty bad IMO */

  /*rss->channel->item->content -> (url) */

  std::vector<std::string> temp_vector;
  std::vector<xmlNode *> item_vector;


  if (node_is(node, "title"))
    {
      std::string title = get_content(node);

      if (title != "")
	{
	  std::cout << "Title: " << title << std::endl;
	}
    }
  

  if (node_is(node, "channel"))    
    {
      item_vector = node_vector(node, "item");
    }

  if (item_vector.size() != 0)
    {
      std::vector<xmlNode *>::iterator it; 

      for (it = item_vector.begin(); it != item_vector.end(); it++)
	{
	  std::vector<xmlNode *> content_vector = node_vector(*it, "content");

	  if (content_vector.size() != 0)
	    {
	      std::vector<xmlNode *>::iterator c_iter;

	      for (c_iter = content_vector.begin(); c_iter != content_vector.end(); c_iter++)
		{
		  std::string link = get_prop(*c_iter, "url");
		  
		  if (link != "")
		    {
		      temp_vector.push_back(link);
		    }
		}
	    }
	}
    }

  return temp_vector;
}






std::string parser::get_content(xmlNode *node)
{
  std::string return_s;
  
  if (node)
    {
      xmlChar *content = xmlNodeGetContent(node);
      if (content)
	{
	  return_s = (const char *)content;
	  xmlFree(content);
	}
    }
  return return_s;
}




xmlNode *get_node(xmlNode *node, const char *value)
{
  xmlNode *ret_node;
  if (node)
    {
      for (xmlNode *c_node = node->children; c_node != NULL; c_node = c_node->next)
	{
	  if (strcmp((const char *)c_node->name, value) == 0)
	    {
	      ret_node = c_node;
	      return ret_node;
	    }
	}
    }
  return ret_node;
}

int parser::parse_feed(xmlNode *root_node)
{
  if (!root_node)
    {
      std::cout << "Error: no root node" << std::endl;
    }

  for (xmlNode *node = root_node->children; node != NULL; node = node->next)
    {
      parser::get_links(node);
      std::cout << node->name << std::endl;
    }  
}


int parse_xmlnode(xmlNode *node)
{
  if (node)
    {
      if (node->name && node->type == XML_ELEMENT_NODE)
	{
	  if (strcmp((const char *)node->name, "rss") == 0)
	    {
	      std::cout << "XML is ok" << std::endl;
	      return 0;
	    }
	  else
	    {
	      std::cout << "Bad XML" << std::endl;
	      return 1;
	    }
	}
    }
}


int parser::parse_buffer(const char *buffer, size_t size, const char *url)
{
  doc = xmlReadMemory(buffer, size, url, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

  if (doc == NULL)
    {
      std::cout << "Could not parse buffer" << std::endl;
      return 1;
    }

  xmlNode *root_element = xmlDocGetRootElement(doc);
  
  if (parse_xmlnode(root_element) == 0)
    {
      parse_feed(root_element);
    }

}

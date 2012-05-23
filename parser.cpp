
#include "parser.hpp"



std::string parser::get_attr(xmlNode *node, const char *attr)
{
  std::string return_s;
  if (node)
    {
      xmlChar *value;
      
      value = xmlGetProp(node, (xmlChar *)attr);
      
      if (value)
	{
	  return_s = (const char *)value;
	  xmlFree(value);
	}
    }
  return return_s;
}



bool parser::node_is(xmlNode *node, const char * name)
{
  
  if (strcmp((const char *)node->name, name) == 0)
    {
      if (dbg.current_state())
	{
	  std::cout << "node_is: " << name << std::endl;
	}
      return true;
    }
  return false;
}





std::vector<xmlNode *> parser::node_vector(xmlNode *node, const char *name)
{
  std::vector<xmlNode *> return_vector;

  for (xmlNode *t_node = node->children; t_node != NULL; t_node = t_node->next)
    {
      if (parser::node_is(t_node, name))
	{
	  return_vector.push_back(t_node);
	}
    }
  
  return return_vector;

}





bool parser::root_node_exists()
{
  if (parser::g_root_node != NULL)
    {
      if (dbg.current_state()) { std::cout << "root_node: exists" << std::endl; }
      return true;
    }
  
  if (dbg.current_state()) { std::cout << "root_node: does not exist" << std::endl; }
  return false;  
}




std::string parser::get_title()
{

  std::string return_s;
  xmlNode *channel_node; 

  for (channel_node = parser::g_root_node->children; channel_node != NULL; channel_node = channel_node->next)
    {
      if (parser::node_is(channel_node, "channel"))
	{
	  break;
	}
    }

  
  if (channel_node == NULL)
    {
      return return_s; 
    }
  
  

  for (xmlNode *title_node = channel_node->children; title_node != NULL; title_node = title_node->next)
    {
      if (parser::node_is(title_node, "title"))
	{
	  
	  return_s = parser::get_content(title_node);
	  if (dbg.current_state())
	    {
	      std::cout << "title: " << return_s << std::endl;
	    }
	  break;
	}
    }
	  
  return return_s;
}

xmlNode *parser::get_node(xmlNode *node_with_children, std::string name)
{
  for (xmlNode *node = node_with_children->children; node != NULL; node = node->next)
    {
      if (node_is(node, name.c_str()))
	{
	  return node;
	}
    }
}
	 

int parser::get_enclosures(std::vector<xmlNode *> *vect)
{
  std::vector<xmlNode *>::iterator temp_iter;
  std::vector<xmlNode *> *our_vect = new std::vector<xmlNode *>; 
  
  

  for (temp_iter = vect->begin(); temp_iter != vect->end(); temp_iter++)
    {
      parser::get_node(*temp_iter, "enclosure");
    }

  
}

int parser::get_all_links(podcast_container *p_container)
{
  std::vector<xmlNode *> *item_vector = new std::vector<xmlNode*>;
  std::vector<xmlNode *> enclosure_vector;

  /* Get all the item nodes. Currently it creates a temp_vector with items, then adds 
     them to the item_vector. When I get more time to look at it, I will see if this is even 
     necessary. */
  for (xmlNode *node = parser::g_root_node->children; node != NULL; node = node->next)
    {
      
      if (!node_is(node, "channel"))
	{
	  continue;
	}
      
      /* Make this a pointer, because its contents can be deleted (I think) */
      std::vector<xmlNode *> temp_vector = node_vector(node, "item");

      if (temp_vector.size())
	{
	  if (dbg.current_state())
	    {
	      std::cout << "temp items: " << temp_vector.size() << std::endl;
	    }
	  item_vector->insert(item_vector->end(), temp_vector.begin(), temp_vector.end());
	}
    }
  
  
  if (dbg.current_state())
    {
      std::cout << "total items: " << item_vector->size() << std::endl;
    }
  
  

  std::vector<xmlNode *>::iterator temp_iter; 

  for (temp_iter = item_vector->begin(); temp_iter != item_vector->end(); temp_iter++)
    {
      xmlNode *e_node = get_node(*temp_iter, "enclosure");
      
      std::string link   = get_attr(e_node, "url");
      std::string format = get_attr(e_node, "type");
     
      if (link.size() && format.size())
	{
	  p_container->media_urls[link] = format;
	}

      else if (link.size())
	{
	  p_container->media_urls[link];
	}
    }      

  delete item_vector;

  return 0;
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





int parser::parse_feed(std::string page, std::string url)
{
  
  const char *c_page = page.c_str();
  const char *c_url  = url.c_str();


  xmlNode *root_node = parser::parse_buffer(c_page, page.size(), c_url);

  if (!root_node)
    {
      std::cout << "Error: could not parse feed (no root node)" << std::endl;
      return 1;
    }

  else
    {
      parser::g_root_node = root_node;
    }

  return 0; 
}



xmlNode *parser::parse_buffer(const char *buffer, size_t size, const char *url)
{

  
  xmlNode *return_node; 


  doc = xmlReadMemory(buffer, size, url, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
  
  if (doc == NULL)
    {
      std::cout << "Could not parse buffer" << std::endl;
      return return_node;
    }


  
  xmlNode *root_element = xmlDocGetRootElement(doc);
 
  if (strcmp((const char*)root_element->name, "rss") == 0)
    { 
      return_node = root_element;
    }
  
  else
    {
      std::cout << "Unknown feed type" << std::endl;
    }

  return return_node;
  
}

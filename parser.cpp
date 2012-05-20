
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
	  break;
	}
    }
	  
  return return_s;
}
	  


int parser::get_all_links(podcast_container *p_container)
{      

  for (xmlNode *node = parser::g_root_node->children; node != NULL; node = node->next)
    {
      
      if (parser::node_is(node, "channel"))
	{ 	  
	  parser::get_links_from_node(node, p_container);
	}
    }
  return 0;
}



int parser::get_links_from_node(xmlNode *node, podcast_container *p_container)
{
  /* I tried to make this as pretty as possible, and it still looks pretty bad IMO */

  /*rss->channel->item->content -> (url) */

  //1 == error
  //0 == ok
  
  std::vector<xmlNode *> item_vector;
  

  /* The first thing we are looking for is channel,
     if it is found, then create a vector of items 
     from the channel node */
  if (parser::node_is(node, "channel"))    
    {
      item_vector = parser::node_vector(node, "item");
      
    }

  else
    {
      std::cout << "Given non-channel node" << std::endl;
      return 1;
    }

  

  /* If anything is in the item vector, then 
     iterate through it and try to grab the links */
  if (item_vector.size() == 0)
    {
      std::cout << "No items found" << std::endl;
      return 1;
    }
  

  std::vector<xmlNode *>::iterator it; 

  
  for (it = item_vector.begin(); it != item_vector.end(); it++)
    {
      /* The content nodes hold the links */

      std::vector<xmlNode *> content_vector = parser::node_vector(*it, "content");
            
      if (content_vector.size() == 0)
    
	{
	  
	  continue;
	}
	  
      
      std::vector<xmlNode *>::iterator c_iter;
      
      for (c_iter = content_vector.begin(); c_iter != content_vector.end(); c_iter++)
	{
	  
	  /* We can now grab the link */
	  
	  std::string link   = parser::get_attr(*c_iter, "url");
	  std::string format = parser::get_attr(*c_iter, "type");

	  if (link.size() && format.size())
	    {
	      p_container->formats[link] = format;
	    }

	  else if (link.size())
	    {
	      p_container->formats[link];

	    }
    	}
    }
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

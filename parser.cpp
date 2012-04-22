
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
      std::cout << "Node is " << name << std::endl;
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









std::vector<std::string> parser::get_links(xmlNode *node)
{
  /* I tried to make this as pretty as possible, and it still looks pretty bad IMO */

  /*rss->channel->item->content -> (url) */

  std::vector<std::string> temp_vector;
  std::vector<xmlNode *> item_vector;



  /* Grab the title */
  if (parser::node_is(node, "title"))
    {
      std::string title = parser::get_content(node);

      if (title != "")
	{
	  /* First item in the vector will be the title */
	  parser::link_vector.push_back(title);
	  std::cout << "Title: " << title << std::endl;
	}
    }
  

  /* The first thing we are looking for is channel,
     if it is found, then create a vector of items 
     from the channel node */
  if (parser::node_is(node, "channel"))    
    {
      item_vector = parser::node_vector(node, "item");
    }


  /* If anything is in the item vector, then 
     iterate through it and try to grab the links */
  if (item_vector.size() != 0)
    {
      std::vector<xmlNode *>::iterator it; 

      
      for (it = item_vector.begin(); it != item_vector.end(); it++)
	{
	  /* The content nodes hold the links */
	  std::vector<xmlNode *> content_vector = parser::node_vector(*it, "content");

	  if (content_vector.size() != 0)
	    {
	      std::vector<xmlNode *>::iterator c_iter;

	      for (c_iter = content_vector.begin(); c_iter != content_vector.end(); c_iter++)
		{

		  /* We can now grab the link */
		  std::string link = parser::get_attr(*c_iter, "url");
		  
		  if (link != "")
		    {
		      parser::link_vector.push_back(link);
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





int parser::parse_feed(std::string page, std::string url)
{
  
  const char *c_page = page.c_str();
  const char *c_url  = url.c_str();


  xmlNode *root_node = parser::parse_buffer(c_page, page.size(), c_url);

  if (!root_node)
    {
      std::cout << "Error: no root node" << std::endl;
      return 1;
    }

  for (xmlNode *node = root_node->children; node != NULL; node = node->next)
    {
      parser::get_links(node);
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
      std::cout << "Buffer ok" << std::endl;
      return_node = root_element;
    }
  
  else
    {
      std::cout << "Unknown feed" << std::endl;
    
    }

  return return_node;
  
}

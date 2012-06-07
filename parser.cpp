
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
      if (debug::state)
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


std::string parser::get_title()
{

  std::string return_s;
  xmlNode *channel_node; 

  for (channel_node = parser::root_node->children; channel_node != NULL; channel_node = channel_node->next)
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
	 
	  if (debug::state)
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


std::map<std::string, std::string> parser::get_links()
{
  /* FIX THIS (NEAR BOTTOM) */
  podcast_container *p_container = new podcast_container;
  
  std::vector<xmlNode *> *item_vector = new std::vector<xmlNode*>;
  std::vector<xmlNode *> enclosure_vector;

  /* Get all the item nodes. Currently it creates a temp_vector with items, then adds 
     them to the item_vector. When I get more time to look at it, I will see if this is even 
     necessary. */
  for (xmlNode *node = parser::root_node->children; node != NULL; node = node->next)
    {
      
      if (!node_is(node, "channel"))
	{
	  continue;
	}
      
      /* Make this a pointer, because its contents can be deleted (I think) */
      std::vector<xmlNode *> temp_vector = node_vector(node, "item");

      if (temp_vector.size())
	{
	  if (debug::state)
	    {
	      std::cout << "temp items: " << temp_vector.size() << std::endl;
	    }
	  item_vector->insert(item_vector->end(), temp_vector.begin(), temp_vector.end());
	}
    }
  
  
  if (debug::state)
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

  return p_container->media_urls;
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





int parser::parse_feed()
{
  
  const char *c_data = parser::data->c_str();
  const char *c_url  = parser::url.c_str();


  xmlNode *r = parser::parse_buffer(c_data, data->size(), c_url);

  if (!r)
    {
      std::cout << "Error: could not parse feed (no root node)" << std::endl;
      return 1;
    }

  else
    {
      parser::root_node = r;
    }

  return 0; 
}


int parser::set_url(std::string url)
{
  parser::url = url;
}


int parser::set_data(std::string *data)
{
  if (debug::state)
    {
      std::cout << "data->size(): " << data->size() << std::endl;
    }
  
  this->data = data;
}

int parser::delete_data()
{
  if (debug::state)
    {
      std::cout << "freeing data" << std::endl;
    }

  delete this->data;
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


#include "parser.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>


static xmlDoc *doc;
static xmlNode *root_node;


/* parser_utils is too keep all the libxml2 stuff in this file only,
   so we don't have to #include it into any other file */
namespace parser_utils
{
  /* Ugly as shit */
  xmlNode *get_node(xmlNode *node_with_children, std::string name);
  
  std::string get_content(xmlNode *node);
  std::string get_attr(xmlNode *node, const char *attr);
  
  bool node_is(xmlNode *node, const char *name);
  
  std::vector<xmlNode *> node_vector(xmlNode *node, const char *name);
  
  std::map<std::string, std::vector<std::string> > url_map;
}


xmlNode *parser_utils::get_node(xmlNode *node_with_children, std::string name)
{
  xmlNode *return_node;
  for (xmlNode *node = node_with_children->children; node != NULL; node = node->next)
    {
      if (parser_utils::node_is(node, name.c_str()))
        {
          return_node = node;
        }
    }
  return return_node;
}


bool parser_utils::node_is(xmlNode *node, const char *name)
{
  if (strcmp((const char *)node->name, name) == 0)
    {
      return true;
    }

  return false;
}

  
std::vector<xmlNode *> parser_utils::node_vector(xmlNode *node, const char *name)
{

  std::vector<xmlNode *> return_vector;
  
  for (xmlNode *t_node = node->children; t_node != NULL; t_node = t_node->next)
    {
      if (parser_utils::node_is(t_node, name))
	{
	  return_vector.push_back(t_node);
	}
    }

  return return_vector;
}


std::string parser_utils::get_attr(xmlNode *node, const char *attr)
{
  std::string temp_string;
  
  if (node)
    {
      xmlChar *value;
      value = xmlGetProp(node, (xmlChar *)attr);
      
      if (value)
	{
	  temp_string = (const char *)value;
	  xmlFree(value);
	}
    }

  return temp_string;
}


std::string parser_utils::get_content(xmlNode *node)
{
  std::string return_string;
  
  if (node)
    {
      xmlChar *content = xmlNodeGetContent(node);
      
      if (content)
	{
	  return_string = (const char *)content;
	  xmlFree(content);
	}
    }

  return return_string;
}
     

/* This is where the public functions are */
void parser::done()
{
  xmlCleanupParser();
  xmlFreeDoc(doc);
}

std::string parser::get_item_title()
{
    std::string return_string;
    xmlNode *channel_node;
    xmlNode *item_node;
    xmlNode *item_title_node;

    if (root_node->children == NULL)
    {
        return return_string;
    }

    channel_node = parser_utils::get_node(root_node, "channel");

    if (!channel_node)
    {
        return return_string;
    }

    item_node = parser_utils::get_node(channel_node, "item");
    
    if (!item_node)
    {
        return return_string;
    }

    item_title_node = parser_utils::get_node(item_node, "title");
    
    if (!item_title_node)
    {
        return return_string;
    }

    return_string = parser_utils::get_content(item_title_node);
    output::msg(1, "item title: %s", return_string.c_str());

    return return_string;
}


std::string parser::get_title()
{

  std::string return_string;
  xmlNode *channel_node;
  xmlNode *title_node;

  if (root_node->children == NULL)
    {
      return return_string;
    }

  channel_node = parser_utils::get_node(root_node, "channel");
  
  if (!channel_node)
    {
      return return_string;
    }


  title_node = parser_utils::get_node(channel_node, "title");
  
  if (!title_node)
    {
      return return_string;
    }

  return_string = parser_utils::get_content(title_node);
  
  output::msg(1, "title: %s", return_string.c_str());

  return return_string;
}


int parser::get_links()
{
  std::vector<xmlNode *> item_vector;

  /* http://cyber.law.harvard.edu/rss/rss.html#requiredChannelElements */
  
  xmlNode *channel_node = parser_utils::get_node(root_node, "channel");
  
  if (!channel_node)
    {
      return 1;
    }
  
  item_vector = parser_utils::node_vector(channel_node, "item");
  
  if (!item_vector.size())
    {
        output::msg(2, "no item nodes");
        return 1;
    }
   
  output::msg(1, "total items: %d", item_vector.size());

  std::vector<xmlNode *>::iterator temp_iter; 

  for (temp_iter = item_vector.begin(); temp_iter != item_vector.end(); temp_iter++)
    {
      xmlNode *e_node = parser_utils::get_node(*temp_iter, "enclosure");
      
      std::string link   = parser_utils::get_attr(e_node, "url");
      std::string format = parser_utils::get_attr(e_node, "type");

      if (link.size() && format.size())
	{
	  format_map[link] = format;
	}
      
      if (link.size())
	{
	  link_vector.push_back(link);
	}
    }   
}


int parser::set_url(std::string url)
{
  this->url = new std::string;
  *this->url = url;
}


int parser::set_data(std::string *data)
{
    output::msg(1, "data size: %d", data->size());
    this->data = data;
}


int parser::parse_feed()
{
  
  if (!this->data->size())
    {
        output::msg(2, "no data from: %s", *parser::url->c_str());
        return 1;
    }

  const char *c_data = parser::data->c_str();
  const char *c_url  = parser::url->c_str();
  size_t size = parser::data->size();
  
  doc = xmlReadMemory(c_data, size, c_url, NULL, XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

  if (doc == NULL)
    {
        output::msg(2, "could not parse buffer from: %s", *parser::url->c_str());
        return 1;
    }
 
  xmlNode *root_element = xmlDocGetRootElement(doc);
 
  if (strcmp((const char*)root_element->name, "rss") == 0)
    { 
      root_node = root_element;
    }
  
  else
    {
        output::msg(2, "could not get feed type from: ", *parser::url->c_str());
        return 1; 
    }

  return 0;
}

parser::~parser()
{
  if (parser::url->size())
    {
      delete parser::url;
    }
}

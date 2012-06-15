
#include "container.hpp"

/* Possibly make container a struct then add it to core */

container::container()
{
  this->title = new std::string;
  this->data  = new std::string;
  this->url   = new std::string;
}


int container::set_data(std::string *data)
{
  this->data = data;
}

int container::set_url(std::string url)
{
  this->url = new std::string;
  *this->url = url;
}

int container::set_title(std::string title)
{
  this->title = new std::string;
  *this->title = title;
}

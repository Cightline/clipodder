
#include "container.hpp"

int container::set_title(std::string title)
{
  this->title = new std::string;
  *this->title = title;
}

int container::set_url(std::string url)
{
  this->url = new std::string;
  *this->url = url;
}

int container::done()
{

  
}

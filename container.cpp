
#include "container.hpp"

int container::set_url(std::string url)
{
  container::url = url;
}

int container::add_media_url(std::string url, std::string parsed_format)
{
  if (parsed_format.size())
    {
      container::media_urls[url] = parsed_format;
    }

  else
    {
      container::media_urls[url];
    }
}

int container::add_format(std::string format)
{
  container::format_vector.push_back(format);
}

int container::set_title(std::string title)
{
  container::title = title;
}

int container::set_save_dir(std::string dir)
{
  container::save_dir = dir;
} 

int container::set_data(std::string *data)
{
  container::data = data;
}

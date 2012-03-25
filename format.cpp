
#include "format.hpp"


void format::setup_formats()
{
  format::k_formats.push_back("mp4");
  format::k_formats.push_back("mp3");
  format::k_formats.push_back("wav");
}



int format::known_format(std::string format)
{
  /* Returns 0 if it is a known format */
  std::vector<std::string>::iterator iter;

  for (iter = format::k_formats.begin(); iter != format::k_formats.end(); iter++)
    {
      if (format == *iter)
        return 0;
    }

  return 1;
}


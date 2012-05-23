
#include "format.hpp"


std::string format::get_extension(std::string media_url)
{
  std::string extension;
  int index;

  index = media_url.find_last_of(".");
  
  if (index)
    {
      extension = media_url.substr(index+1, media_url.size());
    }

  return extension;
}

std::string format::determine_format(std::string media_url)
{
  std::string extension;
  extension = get_extension(media_url);
  
  std::string video = "video";
  std::string audio = "audio";
  std::string no_fmt;

  if (extension == "mp4")
    {
      return video;
    }

  else if (extension == "mp3")
    {
      return "audio";
    }

  else 
    {
      return no_fmt;
    }

}


int format::parse_given_format(std::string to_parse, std::string *format, std::string *extension)
{
  int index = to_parse.find_last_of("/");
  
  if (!index)
    {
      return 1;
    }

  else
    {
      *extension = to_parse.substr(index+1, format->size());
      *format    = to_parse.substr(0, index);
    }

  if (!extension->size() || !format->size())
    {
      return 1;
    }

}

bool format::defined_type(std::vector<std::string> f_vector, std::string extension, std::string format)
{
  std::vector<std::string>::iterator f_iter;
  
  for (f_iter = f_vector.begin(); f_iter != f_vector.end(); f_iter++)
    {
      if (*f_iter == format || *f_iter == extension)
	{
	  if (dbg.state())
	    {
	      std::cout << "supported type: " << *f_iter << std::endl;
	    }
	  return true;
	}
   
    }

  return false;
}

int format::get_filename(std::string url, std::string *return_url)
{
  int start = url.find_last_of("/");
  
  if (!start)
    {
      return 1;
    }

  *return_url = url.substr(start+1, url.size());
  
  return 0;

}

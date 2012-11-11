
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


std::string format::get_format(std::string media_url)
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


std::string format::parse_given_extension(std::string to_parse)
{
  std::string return_string; 
  
  int index = to_parse.find_last_of('/');
  
  if (!index)
    {
      return return_string;
    }

  return_string = to_parse.substr(index+1, to_parse.size());

  return return_string;
}

std::string format::parse_given_format(std::string to_parse)
{
  std::string return_string;

  int index = to_parse.find_last_of("/");
  
  if (!index)
    {
      return return_string;
    }

  return_string = to_parse.substr(0, index);
  
  return return_string;
}  


bool format::defined_type(std::string media_url,
			  std::string supplied_type,
			  std::vector<std::string> to_compare)
			  
{
  std::string file_extension = get_extension(media_url);
  std::string file_format    = get_format(media_url);
  std::string format         = parse_given_format(supplied_type);
  std::string extension      = parse_given_extension(supplied_type);

  std::string matched_type;

  std::vector<std::string>::iterator f_iter;
  
  for (f_iter = to_compare.begin(); f_iter != to_compare.end(); f_iter++)
    {
      if (*f_iter == format)
	{
	  matched_type = format;
	  break;
	}
      if (*f_iter == extension)
	{
	  matched_type = extension;
	  break;
	}
      if (*f_iter == file_format)
	{
	  matched_type = file_format;
	  break;
	}
      if (*f_iter == file_extension)
	{
	  matched_type = file_extension;
	  break;
	}


    }

  if (matched_type.size())
    {
        output::msg(1, "supported type: %s", matched_type.c_str());
        return true;
    }

  return false;
}

std::string format::get_filename(std::string url)
{
  std::string return_string;
  int start = url.find_last_of("/");
  
  if (!start)
    {
      return return_string;
    }

  return_string = url.substr(start+1, url.size());  
  return return_string;
}



#include "core.hpp"


int core::parse_config()
{
  
  std::cout << "Reading config" << std::endl;

  if (core::cfg.parse_config() == 0)
    {
      core::urls = core::cfg.current_urls();
      
      std::string save_path = "/.clipodder/downloads";

      core::save_dir = core::cfg.get_home() + save_path;
      
      std::cout << "Full save path: " << core::save_dir << std::endl;

      if (core::fs.is_dir(core::save_dir))
	{
	  std::cout << "Directory found" << std::endl;
	}

      else
	{
	  std::cout << "Directory does not exist" << std::endl;
	}

      return 1;
    }

  return 0;
}


int core::fill_container(podcast_container *container)
{
  container->data = core::net.fetch_page(container->url);
  
  if (!container->data.size())
    {
      std::cout << "No data from " << container->url << std::endl;
      return 1;
    }

  if (!core::ps.parse_feed(container->data, container->url) == 0)
    {
      std::cout << "Could not parse feed" << std::endl;
      return 1;
    }

  container->title = ps.get_title();

  if (!core::ps.root_node_exists())
    {
      std::cout << "Could not get root node" << std::endl;
      return 1;
    }

  if (core::ps.get_all_links(container) == 0)
    {
      std::cout << "Got " << container->formats.size() << " links" << std::endl;
    }

  else
    {
      std::cout << "Could not get any links, skipping" << std::endl;
      return 1;
    }

  container->max_downloads = core::cfg.max_downloads_map[container->url];
  
  if (!container->formats.size())
    {
      std::cout << "No links found in container" << std::endl;
      return 1;
    }
}





int core::download_podcasts(std::string url)
{
           
  podcast_container *p_container;
  p_container = new podcast_container;
      
      
  p_container->url  = url;
  

  /* Once we assign the struct a url, we can fill it
     and do the rest of the work. 
  */
  if (core::fill_container(p_container) == 1)
    {
      delete p_container;
      return 1;
    }   
  
  
  
  std::cout << "Working on: " << p_container->title << std::endl;	  
  
    
  int counter = 0;
  int file_exists = 0;
  
  
  std::cout << "Max downloads is " << p_container->max_downloads << std::endl;
  
  for (core::m_iter = p_container->formats.begin(); core::m_iter != p_container->formats.end(); core::m_iter++)
    { 
      
      std::string supplied_format;
      
      /* Check to see if we are exceeding max_downloads
	 I could just add this to the for loop, but then it gets
	 too messy */
      if (!counter < p_container->max_downloads)
	{
	  std::cout << "not exceeding max_downloads (" << p_container->max_downloads << "), next..." << std::endl;
	  break;
	}
      
      if (file_exists >= p_container->max_downloads)
	{
	  std::cout << "Downloaded files: " << file_exists << std::endl;
	  break;
	}
      
      // If we found the format during parse
      if (m_iter->second.size())
	{
	  supplied_format = m_iter->second; 
	}
      
      int status;
      
      if (core::should_download(p_container->url, m_iter->first, supplied_format))
	{
	  
	  status = core::deal_with_link(m_iter->first, p_container->title);
	}
      
      if (status == 2)
	{
	  file_exists++;
	}
      
      counter++;
      
    }
  
}



   


std::string core::get_extension(std::string media_url)
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
      



std::string core::determine_format(std::string media_url)
{
  std::string extension;
  
  extension = core::get_extension(media_url);
  
  std::string video = "video";
  std::string audio = "audio";
  std::string no_fmt;

  if (extension == "mp4")
    {
      return "video";
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
  
int core::parse_given_format(std::string to_parse, std::string *format, std::string *extension)
{
  if (!to_parse.size())
    {
      return 1;
    }

  
  int index = to_parse.find_last_of("/");
  
  /* If we could not split it */
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

bool core::defined_type(std::vector<std::string> f_vector, std::string extension, std::string format)
{
 
  std::vector<std::string>::iterator f_iter;

  /* Iterate through the vector to see if the extension or format is defined */
  for (f_iter = f_vector.begin(); f_iter != f_vector.end(); f_iter++)
    {
      if (*f_iter == format || *f_iter == extension)
        {
	  std::cout << "Supported type: " << *f_iter << std::endl;
          return true;
        }
    }

  return false;
}

  
  

bool core::should_download(std::string url, std::string media_url, std::string given_format)
{
  std::string extension;
  std::string format;

  std::string f_format;
  std::string f_extension;
  std::string *s_extension   = new std::string;
  std::string *s_format      = new std::string;
  
  std::vector<std::string> format_vector = cfg.url_map[url];

  /* Uses the media_url to determine if it should return true or false (download the file).
     It will pick the supplied format/extension over the found extension. */
    

  /* If a supplied format is given (video/mp4) */
  
  core::parse_given_format(given_format, s_format, s_extension);
    

  /* Find the format first */
  if (s_format->size())
    {
      format = *s_format;
    }

  else
    {
      /* Attempt to get it from the filename */
      f_format = determine_format(media_url);
      
      if (f_format.size())
	{
	  format = f_format;
	}
      
      else
	{
	  std::cout << "Could not determine format" << std::endl;
	}
    }
      

  /* If we get the supplied extension, use it, otherwise try to use the 
     extension from the filename */
  if (s_extension->size())
    {
      extension = *s_extension;
    }

  /* If we cant use the supplied extension, we try to get the 
     extension from the filename (After we figure out that !s_extension->size()). */

  else
    {
      f_extension = get_extension(media_url);

      if (f_extension.size())
	{
	  extension = f_extension;
	}
      else
	{
	  std::cout << "Could not determine extension" << std::endl;
	}
    
    }

  /* See if the parsed extension or format is in the format vector */
  delete s_extension;
  delete s_format;

  return core::defined_type(format_vector, extension, format);

  
}


int core::get_filename(std::string url, std::string *return_url)
{
  
  int start = url.find_last_of("/");
  
  if (!start)
    {
      return 1;
    }

  
  *return_url = url.substr(start+1, url.size());
  
  return 0;
  
}

int core::prepare_download(std::string final_directory)
{
  
  if (core::fs.is_dir(final_directory) == false)
    {
      std::cout << "Creating directory " << final_directory << std::endl;
      core::fs.make_dir(final_directory);
    }
}



int core::deal_with_link(std::string url, std::string title)
{
  
  std::string final_dir = core::save_dir + "/" + title;
  
  std::cout << "Saving to directory: " << final_dir << std::endl;

  core::prepare_download(final_dir);
  
  std::string *filename;
  filename = new std::string; 
  
  
  if (core::get_filename(url, filename) != 0)
    {
      std::cout << "Could not get filename" << std::endl;
      delete filename;
      return 1;
    }
  
  
  std::string download_path = final_dir + "/" + *filename; 
  std::cout << "Download path: " << download_path << std::endl;
  
  delete filename;


  if (!core::fs.file_exists(download_path))
    {
      std::cout << "Downloading..." << std::endl;
      net.download_file(url, download_path);
    }

  else
    {
      std::cout << "File exists, skipping" << std::endl;
      return 2;
    }
  
}


/*int core::delete_check(std::string path, int max_downloads)
{
  if (!fs.is_dir(path))
    {
      std::cout << "[Core] No such directory: " << path << std::endl;
      return 1;
    }

}
*/
  

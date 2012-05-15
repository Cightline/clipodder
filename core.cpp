
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
	  std::cout << "Format supported, downloading" << std::endl;
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

  if (extension.size())
    {
      if (extension == "mp4")
	{
	  return video;
	}

      if (extension == "m4v")
	{
	  return video;
	}

      if (extension == "wmv")
	{
	  return video;
	}

      if (extension == "mp3")
	{
	  return audio;
	}

      if (extension == "aac")
	{
	  return audio;
	}
    }
      
  else
    {
      return no_fmt;
    }
      
}
  


bool core::should_download(std::string url, std::string media_url, std::string supplied_format)
{

  std::string supposed_extension;
  std::string supposed_format;


  /* If a supplied format is given (video/mp4) split it. */
  if (supplied_format.size())
    {
      std::cout << "Dealing with supplied format: " << supplied_format << std::endl;
      
      int index = supplied_format.find_last_of("/");

      if (index)
	{
	  supposed_extension = supplied_format.substr(index+1, supplied_format.size());
	  supposed_format = supplied_format.substr(0, index);

	  std::cout << "Determined format: " << supposed_format << " extension: " << supposed_extension << std::endl;
	  
	}
      
      
      
      
    }


  std::vector<std::string>::iterator f_iter;
  std::string format;
  std::vector<std::string> f_vector = cfg.url_map[url];
  
  /* If we found the supposed format above */
  if (supposed_format.size())
    {
      format = supposed_format;
    }

  /* If we could not determine the format above 
     try to find the formatit with its extension */
  else
    {
      format = determine_format(media_url);
    }


  /* If something went wrong */
  if (!format.size())
    {
      std::cout << "Could not determine format" << std::endl;
      return false;
    }

  /* Iterate through the vector to see if it exists */
  for (f_iter = f_vector.begin(); f_iter != f_vector.end(); f_iter++)
    {
      if (*f_iter == format)
	{
	  return true;
	}
    }

  return false;

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
  

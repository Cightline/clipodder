
#include "core.hpp"


int core::fill_container(podcast_container *container)
{
  container->data = network::fetch_page(container->url);

  if (!container->data.size())
    {
      std::cout << "Error: no data from " << container->url << std::endl;
      return 1;
    }

  if (!ps.parse_feed(container->data, container->url) == 0)
    {
      std::cout << "Error: could not parse " << container->url << std::endl;
      return 1;
    }

  /* We parsed the feed, so we can grab the title now */
  container->title = ps.get_title();
  
  std::cout << "Checking: " << container->title << std::endl;

  if (!ps.root_node_exists())
    {
      std::cout << "Error: could not get root node" << std::endl;
      return 1;
    }

  
  if (ps.get_all_links(container) != 0)
    {
      std::cout << "Error: could not parse any links, skipping" << std::endl;
      return 1;
    }

    
  if (!container->media_urls.size())
    {
      std::cout << "Error: could not find any links after parsing, skipping" << std::endl;
      return 1;
    }
}


int core::download_podcasts(std::string url, 
			    int max_downloads, 
			    std::string download_dir,
			    std::vector<std::string> format_vector)
{
           
  podcast_container *p_container;
  p_container = new podcast_container;
  
  p_container->url = url;

  /* Once we assign the struct a url, we can fill it
     and do the rest of the work. */
  if (core::fill_container(p_container) == 1)
    {
      delete p_container;
      return 1;
    }   
  
  else if (debug::state)
    {
      std::cout << "links found: " << p_container->media_urls.size() << std::endl;
    }

      
  int counter = 1;
  std::map<std::string, std::string>::iterator media_url;


  /* Ok here we are looking through the found urls. If the format was found during parse (video/mp4), it will be
     m_iter->second. */
  for (media_url = p_container->media_urls.begin(); media_url != p_container->media_urls.end(); media_url++)
    { 
      int download_status;
      std::string supplied_format;
      
      /* Check to see if we are exceeding max_downloads. I could just add counter to the for loop, but then 
	 it gets too messy */
      if (counter > max_downloads)
	{
	  if (debug::state)
	    {
	      std::cout << "not exceeding max_downloads: " << max_downloads << std::endl;
	    }
	  break;
	}
      

      if (debug::state)
	{
	  std::cout << "Iteration: " << counter << std::endl;
	  std::cout << "media_url->first: "  << media_url->first << std::endl;
	  std::cout << "media_url->second: " << media_url->second << std::endl;
	}


      /* If the format is supplied */
      if (media_url->second.size())
	{
	  supplied_format = media_url->second; 
	}
      
                  
      /* If no format was specified (download all) */
      if (!format_vector.size())
	{	  
	  std::string address = media_url->first;

	  if (!core::path_map[address].size())
	    {
	      /* save the path for later use */
	      core::path_map[address] = file_manager::get_final_dir(p_container->title, download_dir);
	    }
	  
	  file_manager::prepare_download(download_dir, core::path_map[address]);
	  download_status = core::deal_with_link(address, p_container->title, core::path_map[address]);
	}
  
      /* Otherwise check the supplied format against the config */	 
      else if (core::should_download(p_container->url, media_url->first, supplied_format, format_vector))
	{
	  std::string address = media_url->first;
	  if (!core::path_map[address].size())
	    {
	      /* save it for later use */
	      core::path_map[address] = file_manager::get_final_dir(p_container->title, download_dir);
	    }

	  file_manager::prepare_download(download_dir, core::path_map[address]);
	  download_status = core::deal_with_link(address, p_container->title, core::path_map[address]);
	}
      
      if (debug::state)
	{
	  std::cout << "download_status: " << download_status << std::endl;
	}

      counter++;
      }

  /* Get rid of this */
  delete p_container;
  return 0;
}


  

bool core::should_download(std::string url, 
			   std::string media_url, 
			   std::string supplied_info,
			   std::vector<std::string> format_vector)
{ 

  std::string format    = format::parse_given_format(supplied_info);
  std::string extension = format::parse_given_extension(supplied_info);
  
  
  
  if (!format.size())
    {
      format = format::determine_format(media_url);
    }

  if (!extension.size())
    {
      extension = format::get_extension(media_url);
    }

  /* Check the format and extension agianst the format_vector (formats in the config file) */
  return format::defined_type(format_vector, extension, format);
}


int core::deal_with_link(std::string media_url, std::string title, std::string final_dir)
{
   
  std::string *filename = new std::string;
  
  *filename = format::get_filename(media_url);

  if (!filename->size())
    {
      std::cout << "Could not get filename from url (" << media_url << ")" << std::endl;
      delete filename;
      return 1;
    }
  
  
  std::string download_path = final_dir + "/" + *filename;
  delete filename;


  /* See if the file exists, if not download */
  if (!filesystem::file_exists(download_path))
    {
      std::cout << "Downloading: " << download_path << std::endl;

      
      int status = network::download_file(media_url, download_path);
      {
	if (debug::state)
	  {
	    std::cout << "status: " << status << std::endl;
	  }
      }
    }

  else
    {
      if (debug::state)
	{
	  std::cout << "already exists: " << download_path << std::endl;
	}

      return 2;
    }
}


  

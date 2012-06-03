
#include "core.hpp"


int core::fill_container(podcast_container *container)
{
  container->data = net.fetch_page(container->url);

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
  
  /* Ok here we are looking through the found urls. If the format was found during parse (video/mp4), it will be
     m_iter->second. */

  for (core::m = p_container->media_urls.begin(); core::m != p_container->media_urls.end(); core::m++)
    { 
      int status;
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
	  std::cout << "m->first: "  << m->first << std::endl;
	  std::cout << "m->second: " << m->second << std::endl;
	}

      // If we found the format during parse
      if (m->second.size())
	{
	  supplied_format = m->second; 
	}
      
      std::string *final_dir = new std::string;
      
      /* If no format was specified */
      if (!format_vector.size())
	{	  
	  dl.prepare_download(p_container->title, m->first, download_dir, final_dir);
	  status = core::deal_with_link(m->first, p_container->title, final_dir);
	}

      /* Otherwise check the format against the config */
      else if (core::should_download(p_container->url, m->first, supplied_format, format_vector))
	{	  
	  dl.prepare_download(p_container->title, m->first, download_dir, final_dir);
	  status = core::deal_with_link(m->first, p_container->title, final_dir);
	}
           
      counter++;
      }

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

  return format::defined_type(format_vector, extension, format);

  
}


int core::deal_with_link(std::string media_url, std::string title, std::string *final_dir)
{
   
  std::string *filename = new std::string;
    
  if (format::get_filename(media_url, filename) != 0)
    {
      std::cout << "Could not get filename from url (" << media_url << ")" << std::endl;
      delete filename;
      delete final_dir;
      return 1;
    }
  
  
  std::string download_path = *final_dir + "/" + *filename; 
    
  delete filename;


  if (!core::fs.file_exists(download_path))
    {
      std::cout << "Downloading: " << download_path <<std::endl;
      int status = net.download_file(media_url, download_path);
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
      delete final_dir;
      return 2;
    }
}


int core::delete_uneeded(std::string path, int max_downloads)
{
  if (!fs.is_dir(path))
    {
      std::cout << "Trying to prune files, but " << path << " does not exist" << std::endl;
      return 1;
    }

  std::vector<std::string> *return_vector = new std::vector<std::string>;

  fs.list_dir(path, return_vector);

}
  

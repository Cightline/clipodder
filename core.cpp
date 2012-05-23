
#include "core.hpp"



bool core::get_config()
{
  if (dbg.state())
    {
      std::cout << "Reading config" << std::endl;
    }

  if (core::cfg.parse_config() == 0)
    {
      return 1;
    }

  return 0;
}




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





int core::download_podcasts(std::string url)
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
  
  else if (dbg.state())
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
      if (counter > cfg.max_downloads_map[p_container->url])
	{
	  if (dbg.state())
	    {
	      std::cout << "not exceeding max_downloads: " << cfg.max_downloads_map[p_container->url] << std::endl;
	    }
	  break;
	}
      
      if (dbg.state())
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


      std::string download_dir;
      std::string *final_dir = new std::string;

      if (cfg.download_map[p_container->url].size())
	{
	  download_dir = cfg.download_map[p_container->url];
	}

      else
	{
	  download_dir = cfg.config_map["download_dir"];
	}

      

      /* If no format was specified */
      if (cfg.url_map[p_container->url][0] == "none")
	{	  
	  dl.prepare_download(p_container->title, m->first, download_dir, final_dir);
	  status = core::deal_with_link(m->first, p_container->title, final_dir);
	}

      /* Otherwise check the format against the config */
      else if (core::should_download(p_container->url, m->first, supplied_format))
	{	  
	  dl.prepare_download(p_container->title, m->first, download_dir, final_dir);
	  status = core::deal_with_link(m->first, p_container->title, final_dir);
	}
            
      counter++;
      
    }
  
  delete p_container;

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
  
  

  fmt.parse_given_format(given_format, s_format, s_extension);
    

  /* Find the format first */
  if (s_format->size())
    {
      format = *s_format;
    }

  else
    {
      /* Attempt to get it from the filename */
      f_format = fmt.determine_format(media_url);
      
      if (f_format.size())
	{
	  format = f_format;
	}
      
      else
	{
	  std::cout << "Warning: could not determine format" << std::endl;
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
      f_extension = fmt.get_extension(media_url);

      if (f_extension.size())
	{
	  extension = f_extension;
	}
      else
	{
	  std::cout << "Warning: Could not determine extension" << std::endl;
	}
    
    }

  /* See if the parsed extension or format is in the format vector */
  delete s_extension;
  delete s_format;

  return fmt.defined_type(format_vector, extension, format);

  
}


int core::deal_with_link(std::string media_url, std::string title, std::string *final_dir)
{
   
  std::string *filename = new std::string;
    
  if (fmt.get_filename(media_url, filename) != 0)
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
	if (dbg.state())
	  {
	    std::cout << "status: " << status << std::endl;
	  }
      }
    }

  else
    {
      if (dbg.state())
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

  fs.list_dir(path);

}
  

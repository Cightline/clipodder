
#include "core.hpp"



bool core::get_config()
{
  if (dbg.current_state())
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
  
  else if (dbg.current_state())
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
	  if (dbg.current_state())
	    {
	      std::cout << "not exceeding max_downloads: " << cfg.max_downloads_map[p_container->url] << std::endl;
	    }
	  break;
	}
      
      if (dbg.current_state())
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

      /* If no format was specified */
      if (cfg.url_map[p_container->url][0] == "none")
	{
	  status = core::deal_with_link(m->first, p_container->title);
	}

      /* Otherwise check the format against the config */
      else if (core::should_download(p_container->url, m->first, supplied_format))
	{
	  status = core::deal_with_link(m->first, p_container->title);
	}
            
      counter++;
      
    }
  delete p_container;

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
	  if (dbg.current_state())
	    {
	      std::cout << "Supported type: " << *f_iter << std::endl;
	    }
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

int core::prepare_download(std::string url, std::string title, std::string *final_dir)
{
  /* NON ERROR CHECKING */

  
  std::string *download_dir = new std::string;

  /* Check to see if its the default */
  if (cfg.download_map[url].size())
    {
      *download_dir = cfg.download_map[url];
    }
  else
    {
      *download_dir = cfg.config_map["download_dir"];
    }
  

  
  /* See if the default directory exists */
  if (fs.is_dir(*download_dir) == false)
    {
      if (dbg.current_state()) 
	{ 
	  std::cout << "Creating: " << *download_dir << std::endl; 
	}
      fs.make_dir(*download_dir);
    }
  
  
  /* Check the directory that the file is being written to */
  *final_dir =  *download_dir + "/" + title;

  if (fs.is_dir(*final_dir) == false)
    {
      if (dbg.current_state())
	{
	  std::cout << "Creating: " << *final_dir << std::endl;
	}
      fs.make_dir(*final_dir);
    }

  if (dbg.current_state())
    {
      std::cout << "download_dir: " << *download_dir << std::endl;
      std::cout << "final_dir: "    << *final_dir    << std::endl;
    }

  return 0;

}



int core::deal_with_link(std::string url, std::string title)
{
  std::string *final_dir = new std::string;

  if (prepare_download(url, title, final_dir) != 0)
    {
      delete final_dir;
      return 1;
    }

  
  std::string *filename = new std::string;
    
  if (core::get_filename(url, filename) != 0)
    {
      std::cout << "Could not get filename from url (" << url << ")" << std::endl;
      delete filename;
      delete final_dir;
      return 1;
    }
  
  
  std::string download_path = *final_dir + "/" + *filename; 
    
  delete filename;


  if (!core::fs.file_exists(download_path))
    {
      std::cout << "Downloading: " << download_path <<std::endl;
      int status = net.download_file(url, download_path);
      {
	std::cout << "Status: " << status << std::endl;
      }

    }

  else
    {
      if (dbg.current_state())
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
  

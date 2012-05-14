
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


int core::validate_urls(std::map<std::string, std::vector<std::string> > urls)
{
  if (urls.size() != 0)
    {
      std::cout << "Downloading podcasts" << std::endl;
      return 0;
    }

  std::cout << "No feeds defined in config" << std::endl;
  return 1;
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

int core::download_podcasts(std::map<std::string, std::vector<std::string> > urls)
{
  
  
  if (core::validate_urls(urls) != 0)
    {
      // Not really a error, just no feeds are defined. 
      return 0;
    }
      

  for (core::u_iter = core::urls.begin(); core::u_iter != core::urls.end(); core::u_iter++)
    {
      
      podcast_container *p_container;
      p_container = new podcast_container;
      
      
      p_container->url  = core::u_iter->first;
      
      if (core::fill_container(p_container) == 1)
	{
	  delete p_container;
	  continue;
	}   
 

      std::cout << "Working on: " << p_container->title << std::endl;	  
      
      std::map<std::string, std::string>::iterator d_iter;

      int counter = 0;
      
      std::cout << "Max downloads is " << p_container->max_downloads << std::endl;
      
      for (d_iter = p_container->formats.begin(); d_iter != p_container->formats.end(); d_iter++)
	{ 
	  if (!counter < p_container->max_downloads)
	    {
	      std::cout << "max_downloads (" << p_container->max_downloads << ")" << std::endl;
	      break;
	    }
	  
	  else if (core::should_download(p_container->url, d_iter->first))
	    {
	      core::deal_with_link(d_iter->first, p_container->title);
	    }

	  counter++;
	  delete p_container;
	}
      
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

    }
      
  else
    {
      return no_fmt;
    }
      
}
  
  


bool core::should_download(std::string url, std::string media_url)
{

  
  std::vector<std::string>::iterator f_iter;

  std::vector<std::string> f_map = cfg.url_map[url];
  std::string format = determine_format(media_url);

  for (f_iter = f_map.begin(); f_iter != f_map.end(); f_iter++)
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




int core::deal_with_link(std::string url, std::string title)
{
  
  std::string final_dir = core::save_dir + "/" + title;
  
  if (!core::fs.is_dir(final_dir))
    {
      std::cout << "Creating directory " << final_dir << std::endl;
      core::fs.make_dir(final_dir);
    }

  
  std::string *filename;
  filename = new std::string; 
  
  
  if (core::get_filename(url, filename) == 0)
    {
      //std::cout << "Filename: " << *filename << std::endl;
    }
  
  else
    {
      std::cout << "Could not get filename" << std::endl;
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
      return 2;
    }
  
}


  

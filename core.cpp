


#include "core.hpp"
#include "network.hpp"
#include "format.hpp"
#include "parser.hpp"
#include "config.hpp"
#include "filesystem.hpp"
#include "downloader.hpp"


config cfg;
network net; 
parser ps;
filesystem fs;
downloader dl;

int core::parse_config()
{
  
  std::cout << "Reading config" << std::endl;

  if (cfg.parse_config() == 0)
    {
      core::urls = cfg.current_urls();
						
      std::string save_path = "/.clipodder/downloads";

      core::save_dir = cfg.get_home() + save_path;
      
      std::cout << "Full save path: " << core::save_dir << std::endl;

      if (fs.is_dir(core::save_dir))
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



int core::download_podcasts()
{
  
  if (core::urls.size() != 0)
    {
      std::cout << "Downloading podcasts" << std::endl;
    }

  for (core::u_iter = core::urls.begin(); core::u_iter != core::urls.end(); core::u_iter++)
    {
      std::string data = net.fetch_page(core::u_iter->first); 
      std::string title;

      if (!data.size())
	{
	  std::cout << "No data from " << core::u_iter->first << std::endl;
	  continue;
	}

      
      
      if (!ps.parse_feed(data, core::u_iter->first) == 0)
	{
	  std::cout << "No feed to parse" << std::endl;
	  continue;
	}

	 
      
      title = ps.get_title();
      
      std::cout << "Working on: " << title << std::endl;
	  

	  
      if (!ps.root_node_exists())
	{
	  std::cout << "No root node, skipping" << std::endl;
	  continue;
	}
	  
      else
	{
	  std::vector<std::string> *links;
	  
	  links = new std::vector<std::string>;
	  
	  if (ps.get_all_links(links) == 0)
	    {
	      std::cout << "Retrived " << links->size() << " links" << std::endl;
	    }

	  else
	    {
	      std::cout << "Error retrieving links, ignoring" << std::endl;
	      continue;
	    }

	  // Construct the path to download

	  
	  int max_downloads = cfg.max_downloads_map[core::u_iter->first] - 1;
	  
	  if (links->size() == 0)
	    {
	      std::cout << "No links found" << std::endl;
	      continue;
	    }
	  

	  for (int c = 0; c <= max_downloads; c++)
	    {
	      // We don't want c to be more than the actual amount of links.
	      
	      if (c > links->size())
		{
		  std::cout << "c is bigger than links" << std::endl;
		  break;
		}
	      
	      core::download_file(links->at(c), title);
	    
	    }
	  
	


	  delete links; 
	}
		  
      
      
    }
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

int core::download_file(std::string url, std::string title)
{
  
  std::string final_dir = core::save_dir + "/" + title;
  
  if (!fs.is_dir(final_dir))
    {
      std::cout << "Creating directory " << final_dir << std::endl;
      fs.make_dir(final_dir);
    }

  
  std::string *filename;
  filename = new std::string; 
  
  
  if (core::get_filename(url, filename) == 0)
    {
      std::cout << "Filename: " << *filename << std::endl;
    }
  
  else
    {
      std::cout << "Could not get filename" << std::endl;
      return 1;
    }
  
  
  std::string download_path = final_dir + "/" + *filename; 
  
  std::cout << "Download path: " << download_path << std::endl;
  
  if (!fs.file_exists(download_path))
    {
      net.download_file(url, download_path);
    }
  
  
}


  

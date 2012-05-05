


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
	 
      

      std::cout << "Working on: " << ps.get_title() << std::endl;
	  
	  
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

	  
	  std::cout << "Downloading " << core::u_iter->first << std::endl;
	  //downloader.download(core::u_iter->first, path, cfg.max_downloads_map[core::u_iter->first]);
	  
	  delete links; 
	}
		  
      
      
    }
}



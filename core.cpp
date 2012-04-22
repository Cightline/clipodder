
/* Include libraries here so I don't have
   to look through the rest of the .hpp
   files to see whats there  */

#include "core.hpp"
#include "network.hpp"
#include "format.hpp"
#include "parser.hpp"
#include "config.hpp"




int core::parse_config()
{
  config cfg; 
  

  std::cout << "Reading config" << std::endl;

  if (cfg.parse_config() == 0)
    {
      core::urls = cfg.current_urls();
      return 1;
    }

  return 0;
}

int core::download_podcasts()
{
  network net;
  parser  ps; 
  if (core::urls.size() != 0)
    {
      std::cout << "Downloading podcasts" << std::endl;
    }

  for (core::u_iter = core::urls.begin(); core::u_iter != core::urls.end(); core::u_iter++)
    {
      std::string data = net.fetch_page(core::u_iter->first); 
      
      if (data.size())
	{
	  if (ps.parse_feed(data, core::u_iter->first) == 0)
	    {
	      std::cout << "Vector size: " << ps.link_vector.size() << std::endl;
	    }
	}


    }

    
}


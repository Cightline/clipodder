

#include "core.hpp"


int main()
{
  
  core clipodder;
  config cfg;

  if (clipodder.get_config() == true)
    {
      
      for (clipodder.u_iter = cfg.url_map.begin(); clipodder.u_iter != cfg.url_map.end(); clipodder.u_iter++)
	{
	  if (clipodder.u_iter->first.size())
	    {
	      
	      clipodder.download_podcasts(clipodder.u_iter->first);
	    }
	  
	}

      
    }
  
  
  return 0;
}

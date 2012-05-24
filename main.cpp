

#include "core.hpp"


int main()
{
  
  core clipodder;
  config cfg;

  if (cfg.parse_config() != 0)
    {
      std::cout << "Could not parse configuration" << std::endl;
      return 1;
    }

  for (clipodder.u_iter = cfg.url_map.begin(); clipodder.u_iter != cfg.url_map.end(); clipodder.u_iter++)
    {
      if (clipodder.u_iter->first.size())
	{
	  clipodder.download_podcasts(clipodder.u_iter->first);
	}
      
    }
  return 0;
}

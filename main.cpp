

#include "main.hpp"

int debug::state = 0;

int main()
{
  
  core clipodder;
  config cfg;

  if (cfg.parse_config() != 0)
    {
      std::cout << "Error: could not parse config" << std::endl;
      return 1;
    }
  
  
  std::map<std::string, std::vector<std::string> >::iterator key;
  
  std::cout << cfg.url_map.size() << std::endl;
  
  for (key = cfg.url_map.begin(); key != cfg.url_map.end(); key++)
    {
      int status;
     
      if (debug::state)
	{
	  std::cout << "url: " << key->first << std::endl;
	}

      /* If no directory was specified */
      if (cfg.download_map[key->first] == "default" || !cfg.download_map[key->first].size())
	{
	  status = clipodder.download_podcasts(key->first, 
					       cfg.max_downloads_map[key->first], 
					       cfg.config_map["download_dir"],
					       key->second);
	}

      /* If a directory was specified */
      else if (cfg.download_map[key->first].size())
	{
	  status = clipodder.download_podcasts(key->first, 
					       cfg.max_downloads_map[key->first],
					       cfg.download_map[key->first],
					       key->second);
	}
      
      if (debug::state)
	{
	  std::cout << "status: " << status << std::endl;
	}
    }
  
    
  
  return 0;
}

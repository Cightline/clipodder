

#include "main.hpp"

/* static */
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
  
  /* Iterate through the urls in the config */
  for (key = cfg.url_map.begin(); key != cfg.url_map.end(); key++)
    {
      unsigned int status = 1;
      
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
      parser_mem::done();
    }
  
  if (debug::state)
    {
      std::cout << "running parser_mem::done()" << std::endl;
    }
  
  


  std::map<std::string, std::string>::iterator p_key; 
  
  if (debug::state)
    {
      std::cout << "deleting excess files" << std::endl;
    }

  for (p_key = clipodder.path_map.begin(); p_key != clipodder.path_map.end(); p_key++)
    {
      if (debug::state)
	{
	  std::cout << "pkey->second " << p_key->second << std::endl;
	}

      file_manager::delete_excess(p_key->second, cfg.max_downloads_map[p_key->first]);
    }
  
  
  
  return 0;
}

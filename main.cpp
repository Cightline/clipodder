

#include "main.hpp"

/* static */
int debug::state = 0;
std::map<std::string, std::string> global_config::config;

int main()
{
  
  core clipodder;
  config_parser cfg;
  
  if (cfg.parse_config() != 0)
    {
      std::cout << "Error: could not parse config" << std::endl;
      return 1; 
    }

  

  std::vector<container> all_podcasts = cfg.get_podcasts();
  std::vector<container>::iterator i;

  if (all_podcasts.size() == 0)
    {
      std::cout << "Error: no podcasts were defined" << std::endl;
      return 0;
    }
 
  /* In a attempt to keep this modular */
  global_config::config["connection-timeout"] = cfg.get_value("connection-timeout");
  global_config::config["show-progress"]      = cfg.get_value("show-progress");
  global_config::config["show-path"]          = cfg.get_value("show-path");

  
  /* Iterate through the urls in the config */
  for (i = all_podcasts.begin(); i != all_podcasts.end(); i++)
    {
     
      int status = clipodder.download_podcasts(*i);

      if (debug::state)
	{
	  std::cout << "clipodder status: " << status << std::endl;
	}
      
      if (i->final_dir != "")
	{
	  file_manager::delete_excess(i->final_dir, i->max_downloads);
	}
      
      delete i->data;

      parser_mem::done();
    }

  cfg.done();

  std::cout << "Done" << std::endl;
  return 0;
}

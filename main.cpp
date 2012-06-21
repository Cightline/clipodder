

#include "main.hpp"

/* static */
int debug::state = 0;

int main()
{
  
  core clipodder;
  config cfg;
  
  
  std::vector<container> all_podcasts = cfg.parse_config();
  std::vector<container>::iterator i;

  if (all_podcasts.size() == 0)
    {
      std::cout << "Error: no podcasts were defined" << std::endl;
      return 0;
    }
  
  
  /* Iterate through the urls in the config */
  for (i = all_podcasts.begin(); i != all_podcasts.end(); i++)
    {
     
      int status = clipodder.download_podcasts(*i);
      i->done();

      
      if (debug::state)
	{
	  std::cout << "status: " << status << std::endl;
	}
      if (debug::state)
	{
	  std::cout << "parser_mem::done()" << std::endl;
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
  
  std::cout << "Done" << std::endl;
  return 0;
}



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
      

      if (debug::state)
	{
	  std::cout << "clipodder status: " << status << std::endl;
	}
      
      if (i->final_dir != "")
	{
	  file_manager::delete_excess(i->final_dir, i->max_downloads);
	}
    }

  parser_mem::done();

  std::cout << "Done" << std::endl;
  return 0;
}



#include "main.hpp"

#include <unistd.h>

/* static */
std::map<std::string, std::string> global_config::config;

int output::suppress = 0;
int output::verbose  = 0;
int output::warnings = 1;

void print_options()
{
  std::string space = "    ";
  std::cout << "Clipodder, a lightweight podcast downloader." << std::endl;

  std::cout << "[options]" << std::endl
	    << space << "-c [conifg_path]"           << std::endl
	    << space << "-v verbose"                 << std::endl
            << space << "-s suppress normal output"  << std::endl
	    << std::endl;
}


int main(int argc, char *argv[])
{
  core clipodder;
  config_parser cfg;

  /* warnings on by default */
  output::warnings = 1;

  /* cli handler */
  int correct_args = 0;
  int index = 1;
  std::string config_path;


  int c;
  while ((c = getopt(argc, argv, "vsc:")) != -1)
  {
     switch (c)
     {
         case 'v':
             output::verbose = 1;
             break;

         case 'c':
             config_path = optarg;
             break;

         case 's':
             output::suppress = 1;
             break;

         case '?':
             if (optopt == 'c')
             {
                 return 1;
             }

             else
             {
                 print_options();
                 return 1;
             }

     
     }
  }


  if (cfg.parse_config(config_path) != 0)
    {
        output::msg(3, "could not parse config");
        return 1; 
    }
  

  std::vector<container> all_podcasts = cfg.get_podcasts();
  std::vector<container>::iterator i;

  if (all_podcasts.size() == 0)
    {
        output::msg(2, "no podcasts defined");
        return 0;
    }
 
  /* In a attempt to keep this modular */
  global_config::config["connection-timeout"] = cfg.get_value("connection-timeout");
  global_config::config["show-progress"]      = cfg.get_value("show-progress");
  global_config::config["show-path"]          = cfg.get_value("show-path");
  
  
  /* Iterate through the urls in the config */
  for (i = all_podcasts.begin(); i != all_podcasts.end(); i++)
    {

      clipodder.download_podcasts(*i);
     
      
      if (i->final_dir != "")
	{
	  file_manager::delete_excess(i->final_dir, i->max_downloads);
	}
      
      delete i->data;
    }

  cfg.done();

  output::msg(0, "Finished");
  return 0;
}

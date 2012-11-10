

#include "main.hpp"

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
	    << space << "--config [path] (path to configuration file)"   << std::endl
	    << space << "--verbose       (output everything)"            << std::endl
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

  /* Iterate through the args */
  while (index < argc)
    {

      std::string current_opt = argv[index];

      if (current_opt == "--help" || current_opt == "-h")
	{
	  print_options();
	  return 0;
	}
      
      else if (current_opt == "--verbose")
	{
	  ++correct_args;
          output::verbose = 1;
	}

      else if (current_opt == "--config")
	{
	  correct_args = correct_args + 2;
	  
	  /* If the next arg is within bounds */
	  if ((index + 1) <= argc -1)
	    {
	      config_path = argv[index + 1]; 
	    }

	  else
	    {
              output::msg("specify a path for config when using --config", 2);
	      return 1;
	    }
	}
      index = index + 1;
    }


  if (correct_args < argc - 1)
    {
      print_options();
      return 1;
    }


  if (cfg.parse_config(config_path) != 0)
    {
        output::msg("Error: could not parse config", 3);
        return 1; 
    }
  

  std::vector<container> all_podcasts = cfg.get_podcasts();
  std::vector<container>::iterator i;

  if (all_podcasts.size() == 0)
    {
        output::msg("No podcasts defined", 2);
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

  output::msg("Done", 0);
  return 0;
}



#include "main.hpp"

/* static */
int debug::state = 0;
std::map<std::string, std::string> global_config::config;

int main(int argc, char *argv[])
{
  

  core clipodder;
  config_parser cfg;


  /* cli handler */
  int correct_args = 0;
  /* so we compare it */
  const char *config = "--config";
  const char *debug  = "--debug";
  std::string config_path = "";
  
  int index = 0;

  /* Iterate through the args */
  while (index < argc)
    {
      if (*argv[index] == *config && (index + 1) < argc && !config_path.size())
	{
	  correct_args = correct_args + 2;
	  config_path = argv[index + 1];
	}
	

      else if (*argv[index] == *debug && debug::state == 0)
	{
	  correct_args = correct_args + 1;
	  debug::state = 1;
	}

      index = index + 1;
    }
  

  /* argc[0] is the filename, so we minus 1, to get the "acutal" number of args */
  if (correct_args != argc - 1)
    {
      std::cout << "c: " << correct_args << std::endl;
      std::cout << "argc: " << argc << std::endl;
      std::cout << "Clipodder, a lightweight cli podcast downloader "
		<< "with support for arbitrary media types (pdf, html, etc...)\n\n" 
		<< "[options]\n"
		<< "   --config [path] (path to configuration file)"
		<< "\n\n";
      return 1;
    }


  if (cfg.parse_config(config_path) != 0)
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
      
    }

  cfg.done();

  std::cout << "Done" << std::endl;
  return 0;
}

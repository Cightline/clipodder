
/* Configuration stuff */

#include "config.hpp"

std::map<std::string, std::vector<std::string> > config::url_map;
std::map<std::string, int> config::max_downloads_map;

std::map<std::string, std::string> config::config_map;
std::map<std::string, std::string> config::download_map;

int config::parse_config()
{
  /* Parses the configuration and puts the                                                                             
     data into url_map */

  
  config_map["home"] = get_home();
  config_map["config_path"] = config_map["home"] + "/.clipodder/config";
  config_map["download_dir"] = config_map["home"] + "/.clipodder/downloads";
  
  

  cfg_opt_t urls[] =
    {
      CFG_STR_LIST("formats", "none", CFGF_NONE),
      CFG_STR("download_dir", "default", CFGF_NONE),
      CFG_INT("max_downloads", 1, CFGF_NONE),
      CFG_END()
    };
      
  cfg_opt_t opts[] =
    {
      CFG_SEC("url", urls, CFGF_TITLE | CFGF_MULTI),
      CFG_INT("debug", 0, CFGF_NONE),
      CFG_STR("download_dir", "default", CFGF_NONE),
      CFG_END()
    };


  cfg_t *cfg, *cfg_url;

  cfg = cfg_init(opts, CFGF_NONE);
  
    

  if(cfg_parse(cfg, config_map["config_path"].c_str()) == CFG_PARSE_ERROR)
    {
      std::cout << "Error parsing configuration" << std::endl;
      return 1;
    }
  
  /* Set the debug state */  
  dbg.set_state(cfg_getint(cfg, "debug"));
  

  /* If the download_dir option is set, reset the default to this */
  std::string default_dir = cfg_getstr(cfg, "download_dir");

  if (default_dir != "default")
    {
      config_map["download_dir"] = cfg_getstr(cfg, "download_dir");
    }
  


  /* Urls will be the title */
  int total_urls = cfg_size(cfg, "url");

  if (dbg.state())
    {
      std::cout << total_urls << " urls found" << std::endl;
    }
  

  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map */

  /* This is per url */
  for (int i = 0; i < total_urls; i++)
    {
      cfg_url = cfg_getnsec(cfg, "url", i);
      
      std::string addr         = cfg_title(cfg_url);
      std::string download_dir = cfg_getstr(cfg_url, "download_dir");
      int num_formats          = cfg_size(cfg_url, "formats");
    
      if (download_dir != "default")
	{
	  
	  config::download_map[addr] = download_dir;
	}

      for (int b = 0; b < num_formats; b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);
	  config::url_map[addr].push_back(format);
	}

      config::max_downloads_map[addr] = cfg_getint(cfg_url, "max_downloads");
      
    }
  cfg_free(cfg);
  return 0;
}


std::string config::get_home()
{
  //http://stackoverflow.com/questions/2910377/get-home-directory-in-linux-c

  std::string return_s;
  const char *home_dir;

  struct passwd *pw = getpwuid(getuid());

  home_dir = pw->pw_dir;
  
  return_s = home_dir;

  if (dbg.state())
    {
      std::cout << "home dir: " << return_s << std::endl;
    }

  return return_s;
}
  

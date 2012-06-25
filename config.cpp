
/* Configuration stuff */

#include "config.hpp"

std::vector<container> config::parse_config()
{
  std::vector<container> return_vector;
  
  config_map["home"] = get_home();
  config_map["config_path"] = config_map["home"] + "/.clipodder/config";
  config_map["download_dir"] = config_map["home"] + "/.clipodder/downloads";
  config_map["show_progress"] = "false";

  cfg_opt_t urls[] =
    {
      CFG_STR_LIST("formats", "", CFGF_NONE),
      CFG_STR("download_dir", "default", CFGF_NONE),
      CFG_STR("dir_name",     "", CFGF_NONE),
      CFG_INT("num_downloads", 1, CFGF_NONE),
      CFG_INT("no_child_dir", 0, CFGF_NONE),
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
      return return_vector;
    }
  
  /* Set the debug state */  
  debug::state = cfg_getint(cfg, "debug");

  
  std::string default_dir = cfg_getstr(cfg, "download_dir");
  

  if (default_dir != "default")
    {
      config_map["download_dir"] = cfg_getstr(cfg, "download_dir");
    }
  
  /* Urls will be the title */
  int total_urls = cfg_size(cfg, "url");
  

  if (debug::state)
    {
      std::cout << total_urls << " urls found" << std::endl;
    }
  



  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map */

  /* This is per url */
  for (int i = 0; i < total_urls; i++)
    {
      container podcast;
      
      cfg_url = cfg_getnsec(cfg, "url", i);
      
      int num_formats = cfg_size(cfg_url, "formats");

      podcast.url = cfg_title(cfg_url);
      podcast.download_dir  = cfg_getstr(cfg_url, "download_dir");
      podcast.dir_name      = cfg_getstr(cfg_url, "dir_name");
      podcast.no_child_dir  = cfg_getint(cfg_url, "no_child_dir");
      podcast.num_formats   = num_formats;
      podcast.num_downloads = cfg_getint(cfg_url, "num_downloads");

      std::string download_dir = cfg_getstr(cfg_url, "download_dir");

      if (podcast.download_dir == "default")
	{
	  podcast.download_dir = config_map["download_dir"];
	}
      

      for (int b = 0; b < num_formats; b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);
	  podcast.config_formats.push_back(format);
	}

      podcast.max_downloads = cfg_getint(cfg_url, "max_downloads");

      return_vector.push_back(podcast);
    }
  
  

  cfg_free(cfg);
  return return_vector;
}


std::string config::get_home()
{
  //http://stackoverflow.com/questions/2910377/get-home-directory-in-linux-c

  std::string return_s;
  const char *home_dir;

  struct passwd *pw = getpwuid(getuid());

  home_dir = pw->pw_dir;
  
  return_s = home_dir;

  if (debug::state)
    {
      std::cout << "home dir: " << return_s << std::endl;
    }

  return return_s;
}
  


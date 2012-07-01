
/* Configuration stuff */

#include "config_parser.hpp"


std::vector<container> config::parse_config()
{
  std::vector<container> temp_vector;

  config_map["home"] = filesystem::get_home();
  config_map["config_path"] = config_map["home"] + "/.clipodder/config";
  config_map["download_dir"] = config_map["home"] + "/.clipodder/downloads";

  cfg_opt_t urls[] =
    {
      CFG_STR_LIST("formats", "", CFGF_NONE),
      CFG_STR("download_dir", "", CFGF_NONE),
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
      CFG_STR("download_dir", "", CFGF_NONE),
      CFG_STR("connection-timeout", "50", CFGF_NONE),
      CFG_STR("show-progress", "0", CFGF_NONE),
      CFG_STR("show-path", "0", CFGF_NONE),
      CFG_END()
    };


  cfg_t *cfg, *cfg_url;

  cfg = cfg_init(opts, CFGF_NONE);
  
  if(cfg_parse(cfg, config_map["config_path"].c_str()) == CFG_PARSE_ERROR)
    {
      return temp_vector;
    }
  
  /* Set the global debug state */  
  debug::state = cfg_getint(cfg, "debug");
    
  /* Set some globals */
  global_config::config["connection-timeout"] = cfg_getstr(cfg, "connection-timeout");
  global_config::config["show-progress"]      = cfg_getstr(cfg, "show-progress");
  global_config::config["show-path"]          = cfg_getstr(cfg, "show-path");

  /* Set the default dir */
  std::string default_dir = cfg_getstr(cfg, "download_dir");
  if (default_dir.size())
    {
      config_map["download_dir"] = default_dir;
    }

  int total_urls = cfg_size(cfg, "url");
  
  if (debug::state)
    {
      std::cout << total_urls << " urls identified..." << std::endl;
    }
  

  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map. This is per url. */
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

      if (podcast.download_dir == "")
	{
	  podcast.download_dir = config_map["download_dir"];
	}
      
      for (int b = 0; b < num_formats; b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);
	  podcast.config_formats.push_back(format);
	}

      podcast.max_downloads = cfg_getint(cfg_url, "max_downloads");
      temp_vector.push_back(podcast);
    }
  
  

  cfg_free(cfg);
  return temp_vector;
}



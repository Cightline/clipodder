
/* Configuration stuff */

#include "config_parser.hpp"

int config_parser::parse_config(std::string path)
{

    if (path.size())
    {
        output::msg(1, "configuration path: %s", path.c_str()); 
    }

    config_map["home"] = filesystem::get_home();
    config_map["download_dir"] = config_map["home"] + "/.clipodder/downloads";


    if (path.size())
    {
      config_map["config_path"] = path;
    }

    else
    {
      config_map["config_path"] = config_map["home"] + "/.clipodder/config";
    }
  

  cfg_opt_t urls[] =
  {
      CFG_STR_LIST("formats", "", CFGF_NONE),
      CFG_STR("download-dir", "", CFGF_NONE),
      CFG_STR("dir-name",     "", CFGF_NONE),
      CFG_INT("num-downloads", 1, CFGF_NONE),
      CFG_INT("no-child-dir", 0,  CFGF_NONE),
      CFG_INT("max-downloads", 1, CFGF_NONE),
      CFG_STR("set-title",    "", CFGF_NONE),
      CFG_INT("save-as-title", 0, CFGF_NONE),
      CFG_END()
  };
      
  cfg_opt_t opts[] =
    {
      CFG_SEC("url", urls, CFGF_TITLE | CFGF_MULTI),
      CFG_STR("download-dir", "", CFGF_NONE),
      CFG_STR("connection-timeout", "50", CFGF_NONE),
      CFG_STR("show-progress", "0", CFGF_NONE),
      CFG_STR("show-path", "0", CFGF_NONE),
      CFG_END()
    };


  cfg_t *cfg_url;

  this->cfg = cfg_init(opts, CFGF_NONE);
  
  if(cfg_parse(this->cfg, config_map["config_path"].c_str()) == CFG_PARSE_ERROR)
    {
      return 1;
    }
  
  
  /* Set the default dir */
  std::string default_dir = cfg_getstr(cfg, "download-dir");
  if (default_dir.size())
    {
      config_map["download-dir"] = default_dir;
    }

  int total_urls = cfg_size(this->cfg, "url");

  

  output::msg(1, "%d urls configured", total_urls);
  

  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map. This is per url. */
  for (int i = 0; i < total_urls; i++)
    {
      container podcast;
      
      cfg_url = cfg_getnsec(this->cfg, "url", i);
      
      int num_formats = cfg_size(cfg_url, "formats");

      podcast.url = cfg_title(cfg_url);
      podcast.num_formats   = num_formats;
      podcast.download_dir  = cfg_getstr(cfg_url, "download-dir");
      podcast.dir_name      = cfg_getstr(cfg_url, "dir-name");
      podcast.no_child_dir  = cfg_getint(cfg_url, "no-child-dir");
      podcast.num_downloads = cfg_getint(cfg_url, "num-downloads");
      podcast.set_title     = cfg_getstr(cfg_url, "set-title");
      podcast.save_as_title = cfg_getint(cfg_url, "save-as-title");

      std::string download_dir = cfg_getstr(cfg_url, "download-dir");

      if (podcast.download_dir == "")
	{
	  podcast.download_dir = config_map["download-dir"];
	}
      
      for (int b = 0; b < num_formats; b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);
	  podcast.config_formats.push_back(format);
	}

      podcast.max_downloads = cfg_getint(cfg_url, "max-downloads");
      this->all_podcasts.push_back(podcast);
    }
  
  return 0;
}


std::vector<container> config_parser::get_podcasts()
{
  return this->all_podcasts;
}


std::string config_parser::get_value(std::string key)
{
  return cfg_getstr(this->cfg, key.c_str());
}
 

void config_parser::done()
{
  cfg_free(this->cfg);
}

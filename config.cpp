
/* Configuration stuff */

#include "config.hpp"

int config::parse_config()
{
  /* Parses the configuration and puts the                                                                             
     data into url_map */

  std::cout << "Parsing configuration" << std::endl;


  cfg_opt_t urls[] =
    {
      CFG_STR_LIST("formats", "none", CFGF_NONE),
      CFG_END()
    };
      
  cfg_opt_t opts[] =
    {
      CFG_SEC("url", urls, CFGF_TITLE | CFGF_MULTI),
      CFG_END()
    };


  cfg_t *cfg, *cfg_url;

  cfg = cfg_init(opts, CFGF_NONE);

  if(cfg_parse(cfg, "config") == CFG_PARSE_ERROR)
    {
      std::cout << "Error parsing configuration" << std::endl;
      return 1;
    }

  /* Urls will be the title */
  int total_urls = cfg_size(cfg, "url");
  
  std::cout << total_urls << " urls found" << std::endl;
 
  /* Iterate through the urls and add the formats                                        
     and addresses to the url_map */
  for (int i = 0; i < total_urls; i++)
    {
      cfg_url = cfg_getnsec(cfg, "url", i);
      
      std::string addr = cfg_title(cfg_url);

      //std::cout << "Address: " << addr << std::endl;                                                                 

      for (int b = 0; b < cfg_size(cfg_url, "formats"); b++)
	{
	  std::string format = cfg_getnstr(cfg_url, "formats", b);

	  config::url_map[addr].push_back(format);
	}
    }
  
  return 0;
}

std::map<std::string, std::vector<std::string> > config::current_urls()
{
  return config::url_map;
}

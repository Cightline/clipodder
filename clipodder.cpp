
/* Include libraries here so I don't have
   to look through the rest of the .hpp
   files to see whats there  */

#include "clipodder.hpp"


#include "network.hpp"
#include "format.hpp"
#include "parser.hpp"
#include "config.hpp"



int main()
{
  
  config c_config;
  parser c_parser;
  network c_network;

  if (c_config.parse_config() != 0)
    {
      std::cout << "Could not parse configuration" << std::endl;
      return 1;
    }

  
  std::map<std::string, std::vector<std::string> > urls = c_config.current_urls();
  
  std::map<std::string, std::vector<std::string> >::iterator iter;

  for (iter = urls.begin(); iter != urls.end(); iter++)
    {
      
      std::string address = iter->first;
      std::cout << address << std::endl;
      
      /* get the page */
      std::string page = c_network.fetch_page(address);
      
      /* Don't use sizeof(page) */
      size_t page_size = page.length();
      
      

      /* parse the page */
      c_parser.parse_buffer(page.c_str(), page_size, address.c_str());
      
      


      std::vector<std::string> fmt_map = iter->second;
      
      std::vector<std::string>::iterator f_iter;

      
      
      for (f_iter = fmt_map.begin(); f_iter != fmt_map.end(); f_iter++)
	{
	 
	  //where the FUCK is this actually declared a pointer? 
	  std::cout << *f_iter << std::endl;

	  //c_parser.get_link("nothing", *f_iter);
	  
	 
	}
      
      
      
      

      

    }




  return 0;



}
 
  

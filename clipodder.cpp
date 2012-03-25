
/* Include libraries here so I don't have
   to look through the rest of the .hpp
   files to see whats there  */

#include "clipodder.hpp"



/* void print(std::string msg)
{
  std::cout << msg << std::endl;
}


int clipodder::download_podcasts()
{
    
  std::map<std::string, std::vector<std::string> >::iterator iter;
  

  for (iter = clipodder::url_map.begin(); iter != clipodder::url_map.end(); iter++)
  {

    std::string address = iter->first;

    std::vector<std::string>::iterator v_iter;
    std::vector<std::string> format_vector = iter->second;
    
    for (v_iter = format_vector.begin(); v_iter != format_vector.end(); v_iter++)
      {
	
	
	if (clipodder::known_formats(*v_iter))
	  {
	    std::cout << "Working on: " << address << std::endl;
	    
	    std::string page = clipodder::fetch_page(address);
	    
	    if (page.length() > 0)
	      {
		//int clipodder::parse_buffer(const char *buffer, size_t size, const char *url)
		const char *buffer = page.c_str();
		const char *url = address.c_str();
		size_t size = sizeof *buffer;
	     	

		clipodder::parse_buffer(buffer, size, url);		
		
	      }
	    
	    else
	      {
		std::cout << "Could not get page" << std::endl;
	      }
	
	    
	    //const char *buffer, int size, const char* URL, const char * encoding, int options
	    //http://xmlsoft.org/html/libxml-parser.html#xmlReadMemory
	    //doc = xmlReadMemory(buffer, 

	  }
      }
  } 
}

*/





int main()
{
  
  return 0;
}
  

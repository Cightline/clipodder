

#include "core.hpp"


int main()
{
  
  core clipodder;

  if (clipodder.parse_config())
    {
      
      for (clipodder.u_iter = clipodder.urls.begin(); clipodder.u_iter != clipodder.urls.end(); clipodder.u_iter++)
	{
	  if (clipodder.u_iter->first.size())
	    {
	    
	      clipodder.download_podcasts(clipodder.u_iter->first);
	    }
	  
	}

      
    }
  

  return 0;
}



#include "downloader.hpp"


int downloader::prepare_download(std::string title, std::string media_url, std::string parent_dir, 
				 std::string *final_dir)
{
  if (!filesystem::is_dir(parent_dir))
    {
      if (debug::state)
	{
	  std::cout << "creating parent directory: " << parent_dir << std::endl;
	}
      filesystem::make_dir(parent_dir);
    }
  

  /* construct the path */
  *final_dir = parent_dir + "/" + title;


  if (!filesystem::is_dir(*final_dir))
    {
      if (debug::state)
	{
	  std::cout << "creating final dir: " << *final_dir << std::endl;
	}
      filesystem::make_dir(*final_dir);
    }

  return 0;
}


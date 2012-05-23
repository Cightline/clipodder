

#include "downloader.hpp"

int downloader::prepare_download(std::string title, std::string media_url, std::string parent_dir, 
				 std::string *final_dir)
{
  
  if (!fs.is_dir(parent_dir))
    {
      if (dbg.current_state())
	{
	  std::cout << "creating parent directory: " << parent_dir << std::endl;
	}
      fs.make_dir(parent_dir);
    }

  *final_dir = parent_dir + "/" + title;

  if (!fs.is_dir(*final_dir))
    {
      if (dbg.current_state())
	{
	  std::cout << "creating final dir: " << *final_dir << std::endl;
	}
      fs.make_dir(*final_dir);
    }

  return 0;
}

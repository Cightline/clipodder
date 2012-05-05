


#include "core.hpp"


int core::parse_config()
{
  
  std::cout << "Reading config" << std::endl;

  if (core::cfg.parse_config() == 0)
    {
      core::urls = core::cfg.current_urls();
      
      std::string save_path = "/.clipodder/downloads";

      core::save_dir = core::cfg.get_home() + save_path;
      
      std::cout << "Full save path: " << core::save_dir << std::endl;

      if (core::fs.is_dir(core::save_dir))
	{
	  std::cout << "Directory found" << std::endl;
	}

      else
	{
	  std::cout << "Directory does not exist" << std::endl;
	}

      return 1;
    }

  return 0;
}


int core::validate_urls(std::map<std::string, std::vector<std::string> > urls)
{
  if (urls.size() != 0)
    {
      std::cout << "Downloading podcasts" << std::endl;
      return 0;
    }

  std::cout << "No feeds defined in config" << std::endl;
  return 1;
}



int core::download_podcasts(std::map<std::string, std::vector<std::string> > urls)
{
  
  
  if (core::validate_urls(urls) != 0)
    {
      // Not really a error, just no feeds are defined. 
      return 0;
    }
      

  for (core::u_iter = core::urls.begin(); core::u_iter != core::urls.end(); core::u_iter++)
    {
      
      feed *current_feed;
      current_feed = new feed;
      
      // This struct was created to provide clarity.
      current_feed->url  = core::u_iter->first;
      current_feed->data = core::net.fetch_page(current_feed->url);
      
      
      if (!current_feed->data.size())
	{
	  std::cout << "No data from " << current_feed->url << std::endl;
	  continue;
	}

      
      
      if (!core::ps.parse_feed(current_feed->data, current_feed->url) == 0)
	{
	  std::cout << "No feed to parse" << std::endl;
	  continue;
	}
      
      current_feed->title = ps.get_title();
      


      std::cout << "Working on: " << current_feed->title << std::endl;	  
	  
      if (!core::ps.root_node_exists())
	{
	  std::cout << "No root node, skipping" << std::endl;
	  continue;
	}
	  
      else
	{
	  
	  current_feed->links = new std::vector<std::string>; 
	  
	    	  
	  if (core::ps.get_all_links(current_feed->links) == 0)
	    {
	      std::cout << "Retrieved " << current_feed->links->size() << " links" << std::endl;
	    }

	  else
	    {
	      std::cout << "Error retrieving links, skipping feed" << std::endl;
	      continue;
	    }

	  
	  // 0 == 1 (as always with []), so max_downloads may be 0 here, which will download 1 file.
	  current_feed->max_downloads = core::cfg.max_downloads_map[current_feed->url] - 1;
	  
	  if (current_feed->links->size() == 0)
	    {
	      std::cout << "No links found" << std::endl;
	      continue;
	    }
	  
	  
	  for (int c = 0; c <= current_feed->max_downloads && c < current_feed->links->size(); c++)
	    {
	      core::deal_with_link(current_feed->links->at(c), current_feed->title);
	    }
	  
	  // Should I delete both?
	  delete current_feed->links;
	  delete current_feed;
	}
    }
}


int core::get_filename(std::string url, std::string *return_url)
{
  

  int start = url.find_last_of("/");
  
  if (!start)
    {
      return 1;
    }

  
  *return_url = url.substr(start+1, url.size());
  
  return 0;
  
}

int core::deal_with_link(std::string url, std::string title)
{
  
  std::string final_dir = core::save_dir + "/" + title;
  
  if (!core::fs.is_dir(final_dir))
    {
      std::cout << "Creating directory " << final_dir << std::endl;
      core::fs.make_dir(final_dir);
    }

  
  std::string *filename;
  filename = new std::string; 
  
  
  if (core::get_filename(url, filename) == 0)
    {
      //std::cout << "Filename: " << *filename << std::endl;
    }
  
  else
    {
      std::cout << "Could not get filename" << std::endl;
      return 1;
    }
  
  
  std::string download_path = final_dir + "/" + *filename; 
  
  std::cout << "Download path: " << download_path << std::endl;
  
  if (!core::fs.file_exists(download_path))
    {
      net.download_file(url, download_path);
    }
  
  delete filename;
  
}


  

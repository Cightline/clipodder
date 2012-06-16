
#include "core.hpp"



int core::save_download_path(std::string address, std::string path)
{
  if (!core::path_map[address].size())
    {
      if (debug::state)
	{
	  std::cout << "storing download path: " << path << std::endl;
	}
      core::path_map[address] = path;
    }
}


int core::download_podcasts(std::string url, 
			    int max_downloads, 
			    std::string download_dir,
			    std::vector<std::string> format_vector)
{
           
  /* Set up our container to hold the extra info that we want to save for later */
  parser ps;
  
  data_ podcast;

  podcast.title = new std::string;
  podcast.url   = new std::string;
  podcast.data  = network::fetch_page(url);

  *podcast.url = url;


  /* Setup the parser */
  ps.set_url(podcast.url);
  ps.set_data(podcast.data);
  ps.parse_feed();
  
  *podcast.title = ps.get_title();
    
  if (!podcast.title->size())
    {
      std::cout << "Warning: could not get title for url: " << url << std::endl;

      delete podcast.title;
      delete podcast.url; 
      delete podcast.data;

      return 0;
    }

  

  std::cout << "Checking: " << *podcast.title << std::endl;

  /* Iterate through all the found media_urls for this feed and download if needed.
     It was downloading oldest first, so I just reversed the map */

  int counter = 0;
  std::vector<std::string>::iterator i;
  
  ps.get_links();

  for (i = ps.link_vector.begin(); i != ps.link_vector.end(); i++)
    {

      /* Incase we start a iteration, but we are going to exceed max_downloads. 
	 (max_downloads defaults to 1) */
 
      ++counter;
 
      if (counter > max_downloads)
	{
	  if (debug::state) 
	    { 
	      std::cout << "max_downloads: " << max_downloads << " counter: " << counter << std::endl; 
	    }
	  break;
	  }
      
      /* For simplicity */
      std::string file_url      = *i;
      std::string parsed_format = ps.format_map[*i];
      std::string final_dir     = file_manager::get_final_dir(*podcast.title, download_dir);
      unsigned int download_link = 1;
	
      
      
      if (debug::state)
	{
	  std::cout << "file_url: "      << file_url << std::endl
		    << "parsed_format: " << parsed_format << std::endl
		    << "final_dir: "     << final_dir << std::endl;
	}

      core::save_download_path(url, final_dir);

      /* If no formats were specified in the config (download all) */
      if (!format_vector.size())
	{
	  /* Ensure the directories are present */
	  if (file_manager::prepare_download(download_dir, final_dir) == 0)
	    {
	      download_link = core::download_link(file_url, *podcast.title, final_dir);
	    }
	}

      /* Otherwise test it against the found format */
      else if (core::should_download(url, file_url, parsed_format, format_vector))
	{
	  if (file_manager::prepare_download(download_dir, final_dir) == 0)
	    {
	      download_link = core::download_link(file_url, *podcast.title, final_dir);
	    }
	}
	
      if (debug::state)
	{
	  std::cout << "download_link: " << download_link << std::endl;
	}
    }
  
  delete podcast.title;
  delete podcast.url;
  delete podcast.data;
  ps.done();

  return 0;

}

  

bool core::should_download(std::string url, 
			   std::string media_url, 
			   std::string supplied_info,
			   std::vector<std::string> format_vector)
{ 

  std::string format    = format::parse_given_format(supplied_info);
  std::string extension = format::parse_given_extension(supplied_info);
  
  
  
  if (!format.size())
    {
      format = format::determine_format(media_url);
    }

  if (!extension.size())
    {
      extension = format::get_extension(media_url);
    }

  /* Check the format and extension agianst the format_vector (formats in the config file) */
  return format::defined_type(format_vector, extension, format);
}


int core::download_link(std::string media_url, std::string title, std::string final_dir)
{
   
  std::string *filename = new std::string;
  *filename = format::get_filename(media_url);

  if (!filename->size())
    {
      std::cout << "Warning: Could not get filename from url (" << media_url << ")" << std::endl;
      delete filename;
      return 1;
    }
    
  std::string download_path = final_dir + "/" + *filename;
  delete filename;

  /* See if the file exists, if not download */
  if (!filesystem::file_exists(download_path))
    {
      std::cout << "Downloading: " << download_path << std::endl;
      
      int status = network::download_file(media_url, download_path);
      
      if (debug::state)
	{
	  std::cout << "download_status: " << status << std::endl;
	}
    }

  else
    {
      if (debug::state)
	{
	  std::cout << "already exists: " << download_path << std::endl;
	}

      return 2;
    }
}


  

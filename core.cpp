
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


int core::download_podcasts(container &podcast)
{
           
  /* Set up our container to hold the extra info that we want to save for later */
  parser ps;
  
  podcast.data = network::fetch_page(podcast.url);

  /* Setup the parser */
  ps.set_url(podcast.url);
  ps.set_data(podcast.data);

  if (ps.parse_feed() != 0)
    {
      return 1;
    }
  
  podcast.title = ps.get_title();
  

  if (!podcast.title.size())
    {
      std::cout << "Warning: could not get title for url: " << podcast.url << std::endl;
      return 0;
    }

  

  std::cout << "Checking: " << podcast.title << std::endl;

  /* Iterate through all the found media_urls for this feed and download if needed.
     It was downloading oldest first, so I just reversed the map */

  int counter = 0;
  std::vector<std::string>::iterator i;
  
  ps.get_links();

  podcast.link_vector = ps.link_vector;
  

  for (i = podcast.link_vector.begin(); i != podcast.link_vector.end(); i++)
    {

      /* Incase we start a iteration, but we are going to exceed max_downloads. 
	 (max_downloads defaults to 1) */
 
      ++counter;
 
      if (counter > podcast.num_downloads)
	{
	  if (debug::state) 
	    { 
	      std::cout << "num_downloads: " << podcast.num_downloads << " counter: " << counter << std::endl; 
	    }
	  break;
	}
      
      if (podcast.no_child_dir)
	{
	  podcast.final_dir = podcast.download_dir;
	}

      else if (podcast.dir_name.size())
	{
	  podcast.final_dir = file_manager::get_final_dir(podcast.dir_name, podcast.download_dir);
	}

      else 
	{
	  podcast.final_dir = file_manager::get_final_dir(podcast.title, podcast.download_dir); 
	}


      std::string file_url      = *i;
      std::string parsed_format = ps.format_map[*i];
      unsigned int download_link = 1;
      
      if (debug::state)
	{
	  std::cout << "file_url: "      << file_url << std::endl
		    << "parsed_format: " << parsed_format << std::endl
		    << "final_dir: "     << podcast.final_dir << std::endl;
	}

      /* If no formats were specified in the config (download all) */
      if (!podcast.config_formats.size())
	{
	  /* Ensure the directories are present */
	  if (file_manager::prepare_download(podcast.download_dir, podcast.final_dir) == 0)
	    {
	      download_link = core::download_link(file_url, podcast.title, podcast.final_dir);
	    }

	  else
	    {
	      std::cout << "Error: could not prepare download" << std::endl;
	      break;
	    }
	}

      /* Otherwise test it against the found format */
      else if (core::should_download(podcast.url, file_url, parsed_format, podcast.config_formats))
	{
	  if (file_manager::prepare_download(podcast.download_dir, podcast.final_dir) == 0)
	    {
	      download_link = core::download_link(file_url, podcast.title, podcast.final_dir);
	    }
	  else
	    {
	      std::cout << "Error: could not prepare download" << std::endl;
	      break;
	    }
	}
	
      if (debug::state)
	{
	  std::cout << "download_link: " << download_link << std::endl;
	}
    }
  
  

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
  std::string filename = format::get_filename(media_url);

  if (!filename.size())
    {
      std::cout << "Warning: Could not get filename from url (" << media_url << ")" << std::endl;
      return 1;
    }
    
  std::string download_path = final_dir + "/" + filename;

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


  

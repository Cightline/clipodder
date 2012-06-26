
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
  /* A container reference is passed here (contructed by config). This handles 
     mainly all the "non-network" functions. */

  /* See if we can pull the page, if not return error */
  if (debug::state)
    {
      std::cout << "requesting page: " << podcast.url << std::endl;
    }

  podcast.data = network::fetch_page(podcast.url);

  if (!podcast.data->size())
    {
      std::cout << "Warning: no data from: " << podcast.url << std::endl;
      return 1;
    }
  

  /* The parser is not even constructed if the above segment fails (no webpage, no parser) */
  parser ps;
  
  ps.set_url(podcast.url);
  ps.set_data(podcast.data);

  if (ps.parse_feed() != 0)
    {
      return 1;
    }
  

  /* If we can't grab the title, we can't save to some_dir/Title of Feed.
     It does however save if we successfully parsed the feed, and the config 
     states that it does not want a "child_dir". Meaning that we don't need the title
     because we are saving to a directory that already exists. */
  podcast.title = ps.get_title();

  if (!podcast.title.size() && !podcast.no_child_dir)
    {
      std::cout << "Warning: could not get title for url: " << podcast.url << std::endl;
      return 1;
    }


  std::cout << "Checking: " << podcast.title << std::endl;

  int counter = 0;
  std::vector<std::string>::iterator i;
  
  ps.get_links();

  podcast.link_vector = ps.link_vector;
  
  /* Now that most of the sanity checks are done, we can sift through all the found links
     and deal with them. */
  for (i = podcast.link_vector.begin(); i != podcast.link_vector.end(); i++)
    {

      /* Incase we start a iteration, but we are going to exceed max_downloads. 
	 (max_downloads defaults to 1). I could throw this up in the "for" loop but I'm not 
	 trying to clutter everything up */
      ++counter;
 
      if (counter > podcast.num_downloads)
	{
	  if (debug::state) 
	    { 
	      std::cout << "num_downloads: " << podcast.num_downloads << " counter: " << counter << std::endl; 
	    }
	  break;
	}
      
      // some_dir/
      if (podcast.no_child_dir)
	{
	  podcast.final_dir = podcast.download_dir;
	}

      // some_dir/specified_name/ 
      else if (podcast.dir_name.size())
	{
	  podcast.final_dir = file_manager::get_final_dir(podcast.dir_name, podcast.download_dir);
	}

      // some_dir/Tile of Feed 
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
      //else if (core::should_download(podcast.url, file_url, parsed_format, podcast.config_formats))
      else if (format::defined_type(file_url, parsed_format, podcast.config_formats))
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


  

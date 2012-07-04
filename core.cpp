
#include "core.hpp"


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

  else if (!podcast.title.size())
    {
      std::cout << "Checking: (no title) " << podcast.url <<  std::endl;
    }

  else
    {
      std::cout << "Checking: " << podcast.title << std::endl;
    }

  
  ps.get_links();

  int access  = podcast.num_downloads - 1;
  podcast.link_vector = ps.link_vector;

  if (podcast.link_vector.size() < access)
    {
      access = podcast.link_vector.size() - 1;
    }



  /* Now that most of the sanity checks are done, we can sift through all the found links
     and deal with them. We are using "access" as a indexer, which is moving in reverse to the begging 
     of the vector. I do this because we only want to download "podcast.num_downloads", and 
     we need to start out with the oldest first, so the mtimes are written from oldest to latest. */
  while (access > -1)
    {
      std::string file_url = podcast.link_vector.at(access);

      --access;

      if (!core::determine_download_dir(podcast) == 0)
	{
	  std::cout << "Warning: could not determine download directory" << std::endl;
	  return 1;
	}

      std::string parsed_format = ps.format_map[file_url];
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
  std::string download_path;
  std::string filename = format::get_filename(media_url);
  bool show_progress   = false;
  
  if (!filename.size())
    {
      std::cout << "Warning: Could not get filename from url: " << media_url << std::endl;
      return 1;
    }
    
  download_path = final_dir + "/" + filename;

  /* See if the file exists, if not download */
  if (!filesystem::file_exists(download_path))
    {
      
      if (utils::convert_to_bool(global_config::config["show-path"]) == true)
	{
	  std::cout << "Downloading: " << download_path << std::endl;
	}

      else
	{
	  std::cout << "Downloading: " << filename << std::endl;
	}
      
      if (utils::convert_to_bool(global_config::config["show-progress"]) == true)
	{
	  show_progress = true;
	}
      
      int status = network::download_file(media_url, download_path, show_progress);
      
      if (debug::state)
	{
	  std::cout << "network::download_file status: " << status << std::endl;
	}
      
      if (status)
	{
	  std::cout << "Warning: could not download: " << media_url << std::endl;
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


  
int core::determine_download_dir(container &podcast)
{
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

  if (podcast.final_dir.size())
    {
      return 0;
    }
  
  return 1;
}

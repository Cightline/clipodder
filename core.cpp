
#include "core.hpp"


int core::download_podcasts(container &podcast)
{
  /* A container reference is passed here (contructed by config). This handles 
     mainly all the "non-network" functions. */

  std::string downl_warn  = "Warning: could not create download directory";
  std::string final_warn  = "Warning: could not create final directory";

  /* See if we can pull the page, if not return error */
  output::msg(1, "requesting page: %s", podcast.url.c_str());

  podcast.data = network::fetch_page(podcast.url);

  if (!podcast.data->size())
    {
        output::msg(2, "no data from: %s", podcast.url.c_str());
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
        output::msg(2, "could not get the title for url: ", podcast.url.c_str());
        return 1;
    }

  else if (!podcast.title.size())
    {
        output::msg(0, "Querying: (no title) %s", podcast.url.c_str());
    }

  else
    {
        output::msg(0, "Querying: %s", podcast.title.c_str());

    }

 
  ps.get_links();


  int access  = podcast.num_downloads - 1;
  podcast.link_vector = ps.link_vector;

  if (podcast.link_vector.size() < access)
    {
      access = podcast.link_vector.size() - 1;
    }



  /* Now that most of the sanity checks are done, we can sift through all the found links
     and deal with them. We are using "access" as a indexer, which is moving in reverse to the beginning 
     of the vector. I do this because we only want to download "podcast.num_downloads", and 
     we need to start out with the oldest first, so the mtimes are written from oldest to latest. */
  while (access > -1)
    {
      
      std::string file_url = podcast.link_vector.at(access);

      --access;

      if (!core::determine_download_dir(podcast) == 0)
	{
            output::msg(2, "could not determine download directory");
	    return 1;
	}

      std::string parsed_format = ps.format_map[file_url];
      unsigned int download_link = 1;

      output::msg(1, "file_url: %s", file_url.c_str());
      output::msg(1, "parsed_format: %s", parsed_format.c_str());
      output::msg(1, "final_dir: %s", podcast.final_dir.c_str());


      /* If no formats were specified in the config (download all) */
      if (!podcast.config_formats.size())
	{

	  /* Ensure the directories are present */
	  if (file_manager::create_dir(podcast.download_dir) != 0)
	    {
                output::msg(2,"%s", downl_warn.c_str());
	        break;
	    }
	  
	  if (file_manager::create_dir(podcast.final_dir) != 0)
	    {
                output::msg(2, "%s %s", final_warn.c_str(), podcast.final_dir.c_str());
	        break;
	    }

	  download_link = core::download_link(file_url, podcast.title, podcast.final_dir);
	}

      /* Otherwise test it against the found format */
      else if (format::defined_type(file_url, parsed_format, podcast.config_formats))
	{

	  if (file_manager::create_dir(podcast.download_dir) != 0)
	    {
                output::msg(2, "%s", downl_warn.c_str());
	        break;
	    }
	  
	  if (file_manager::create_dir(podcast.final_dir) != 0)
	    {
                output::msg(2,"%s %s",  final_warn.c_str(), podcast.final_dir.c_str());
	        break;
	    }
	  
	  download_link = core::download_link(file_url, podcast.title, podcast.final_dir);
	}
        output::msg(1, "download_link: %d", download_link);
    }
  
  ps.done();

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
      

      show_progress = utils::convert_to_bool(global_config::config["show-progress"]);

      
      int status = network::download_file(media_url, download_path, show_progress);
     
      output::msg(1, "network::download_file status: %d", status);
     

      if (status)
	{
            output::msg(2, "could not download: %s", media_url.c_str());
	}
    }

  else
    {
        output::msg(1, "already exists: %s", download_path.c_str());
        return 2;
    }

  return 0;
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

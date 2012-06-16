
#include "file_manager.hpp"

int file_manager::delete_excess(std::string path, int max_files)
{
  if (!filesystem::is_dir(path))
    {
      std::cout << "Trying to prune files but " << path << " does not exist" << std::endl;
      return 1;
    }
    

  std::vector<std::string> *file_vector = new std::vector<std::string>;
  std::vector<std::string>::iterator f_iter; 

  /* Get the files in the directory */
  filesystem::list_dir(path, file_vector);

  std::map<std::string, int> mtime_map;
  
  for (f_iter = file_vector->begin(); f_iter != file_vector->end(); f_iter++)
    {
      
      std::string file_path = path + "/" + *f_iter; 
      int mtime;

      mtime = filesystem::get_mtime(file_path);

      if (mtime != 0)
	{
	  mtime_map[file_path] = mtime;
	}
    }

  if (debug::state)
    {
      std::cout << "freeing " << file_vector->size() << " bytes" << std::endl;
    }

  delete file_vector;

  std::map<std::string, int>::iterator c;
  
  int counter = 0;

  /* We iterate through and keep a counter of the iteration (files).
     When the counter exceeds max_files, it starts to delete */
  for (c = mtime_map.begin(); c != mtime_map.end(); c++)
    {

      ++counter;

      if (counter > max_files)
	{
	  std::cout << "Deleting: " << c->first << std::endl;
	  filesystem::remove_file(c->first);
	}
      
      if (debug::state)
	{
	  std::cout << "file: "  << c->first << std::endl;
	  std::cout << "mtime: " << c->second << std::endl;
	}
    }
	

}


std::string file_manager::get_final_dir(std::string title, std::string parent_dir)
{
  std::string return_string;

  return_string = parent_dir + "/" + title;

  return return_string;
}


int file_manager::prepare_download(std::string parent_dir, std::string final_dir)
{
  if (!filesystem::is_dir(parent_dir))
    {
      if (debug::state)
	{
	  std::cout << "creating parent directory: " << parent_dir << std::endl;
	}

      filesystem::make_dir(parent_dir);
    }
  
  if (!filesystem::is_dir(final_dir))
    {
      if (debug::state)
	{
	  std::cout << "creating final directory: " << final_dir << std::endl;
	}
      filesystem::make_dir(final_dir);
    }

  return 0;
}

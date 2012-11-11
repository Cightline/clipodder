
#include "file_manager.hpp"

int file_manager::delete_excess(std::string path, int max_files)
{
  if (!filesystem::is_dir(path))
    {
        output::msg(2, "trying to prune files but %s does not exist", path.c_str());
        return 1;
    }
    
  /* Build a map with the mtimes */
  std::vector<std::string> file_vector;
  std::vector<std::string>::iterator f; 

  /* Get the files in the directory */
  filesystem::list_dir(path, file_vector);

  std::map<int, std::string> mtime_map;
  
  for (f = file_vector.begin(); f != file_vector.end(); f++)
    {
      
      std::string file_path = path + "/" + *f; 
      int mtime;

      mtime = filesystem::get_mtime(file_path);

      if (mtime != 0)
	{
	  mtime_map[mtime] = file_path;
	}
    }



  std::map<int, std::string>::reverse_iterator c;
  
  int counter = 0;

  /* We iterate through and keep a counter of the iteration (files).
     When the counter exceeds max_files, it starts to delete */
  for (c = mtime_map.rbegin(); c != mtime_map.rend(); c++)
    {

      ++counter;

      if (counter > max_files)
	{
	  std::cout << "Deleting: " << c->second << std::endl;
	  filesystem::remove_file(c->second);
	}
     
    }
	

}


std::string file_manager::get_final_dir(std::string title, std::string parent_dir)
{
  std::string return_string;

  return_string = parent_dir + "/" + title;

  return return_string;
}


int file_manager::create_dir(std::string dir)
{
  if (filesystem::is_dir(dir) == false)
    {
        output::msg(1, "creating directory: %s", dir.c_str());
      
        if (filesystem::make_dir(dir) == false)
	{
	  return 1;
	}

        else 
	{
	  return 0;
	}
    }
  
  return 0;
}

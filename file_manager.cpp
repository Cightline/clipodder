
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

  filesystem::list_dir(path, file_vector);

  for (f_iter = file_vector->begin(); f_iter != file_vector->end(); f_iter++)
    {
      std::cout << "file: " << *f_iter << std::endl;
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

#include "filesystem.hpp"


int filesystem::list_dir(std::string path, std::vector<std::string> &file_vector)
{
  DIR *dir;
  struct dirent *dir_struct;
  
  dir = opendir(path.c_str());
  
  if (dir == NULL)
    {
      return 1;
    }
  
  while ((dir_struct = readdir(dir)) && dir_struct != NULL)
    {
      std::string *dirname = new std::string;
      *dirname = dir_struct->d_name;
      
      if (*dirname != ".." && *dirname != ".")
	{
	  file_vector.push_back(dir_struct->d_name);
	}
      delete dirname;
    }

  closedir(dir);
  return 0;
}


bool filesystem::file_exists(std::string path)
{

  struct stat results;

  if (stat(path.c_str(), &results) == 0)
    {
      if (S_ISREG(results.st_mode) != 0)
	{
	  return true;
	}
    }

  return false;
}


bool filesystem::is_dir(std::string path)
{
  struct stat results;
  
  if (stat(path.c_str(), &results) == 0)
    {
      if (S_ISDIR(results.st_mode) != 0)
	{
	  return true;
	}
    }
  
  return false;
  
}

bool filesystem::make_dir(std::string path)
{

  /* mkdir() returns -1 on failure */
  int result;

  result = mkdir(path.c_str(), 0700);
  
  if (result == 0)
    {
      return true;
    }

  
  return false;
  
}

int filesystem::get_mtime(std::string file_path)
{
  struct stat results; 

  if (stat(file_path.c_str(), &results) == 0)
    {
      return results.st_mtime;
    }
   
  return 0;

}


int filesystem::remove_file(std::string file_path)
{
  return remove(file_path.c_str());
}


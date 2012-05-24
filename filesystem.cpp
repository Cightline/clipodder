#include "filesystem.hpp"


int filesystem::list_dir(std::string path, std::vector<std::string> *return_vector)
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
	  return_vector->push_back(dir_struct->d_name);
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
      if (S_ISREG(results.st_mode))
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
      if (S_ISDIR(results.st_mode))
	{
	  return true;
	}
    }
  
  return false;
  
}

bool filesystem::make_dir(std::string path)
{
  
  int result;

  result = mkdir(path.c_str(), 0700);
  
  if (result != 0)
    {
      return true;
    }

  
  return false;
  
}

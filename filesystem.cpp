#include "filesystem.hpp"


std::vector<std::string> filesystem::list_dir(std::string path)
{
  const char *dir_name = path.c_str();
  opendir(dir_name);
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

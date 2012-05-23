#include "filesystem.hpp"


std::vector<std::string> filesystem::list_dir(std::string path)
{
  const char *dir_name = path.c_str();
  opendir(dir_name);
}

bool filesystem::file_exists(std::string path)
{

  struct stat results;

  stat(path.c_str(), &results);
  
  if (S_ISREG(results.st_mode) == 0)
    {
      return false;
    }

  return true;
}


bool filesystem::is_dir(std::string path)
{
  struct stat results;
  
  stat(path.c_str(), &results);

  
  if (S_ISDIR(results.st_mode) == 0)
    {
      return false;
    }

  else
    {
      return true;
    }
  
 
}

bool filesystem::make_dir(std::string path)
{
  
  int result;

  result = mkdir(path.c_str(), 0700);
  
  if (result == 0)
    {
      return false;
    }

  else
    {
      return true;
    }
}

#include "filesystem.hpp"


bool filesystem::file_exists(std::string path)
{

  struct stat results; 

  if (stat(path.c_str(), &results) == 0)
    {
      
      return true;

    }

  else
    {
      
      return false;
    }

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

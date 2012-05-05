

#include "core.hpp"


int main()
{
  
  core clipodder;

  if (clipodder.parse_config())
    {
      clipodder.download_podcasts();
    }
  

  return 0;
}

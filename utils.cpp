
#include "utils.hpp"

bool utils::convert_to_bool(std::string to_convert)
{
  if (to_convert == "yes" || to_convert ==  "true" || to_convert ==  "1")
    {
      return true;
    }

  else if (to_convert == "no" || to_convert == "false" || to_convert ==  "0")
    {
      return false;
    }

  return false;
}

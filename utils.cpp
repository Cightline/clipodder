
#include "utils.hpp"

bool utils::convert_to_bool(std::string to_convert)
{
  if (to_convert == "yes" || "true" || "1")
    {
      return true;
    }

  else if (to_convert == "no" || "false" || "0")
    {
      return false;
    }

  return false;
}

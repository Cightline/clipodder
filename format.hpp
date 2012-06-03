
#include <string>
#include <vector>

#include "debug.hpp"

namespace format {
  
  std::string get_filename(std::string url);
  bool defined_type(std::vector<std::string> f_vector, std::string extension, std::string format);

  std::string parse_given_format(std::string to_parse);
  std::string parse_given_extension(std::string to_parse);

  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  
};

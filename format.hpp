#include <iostream>
#include <string>
#include <vector>

#include "output.hpp"
namespace format {
  
  std::string get_filename(std::string url);
  bool defined_type(std::string media_url, std::string supplied_type, std::vector<std::string> to_compare);

  std::string parse_given_format(std::string to_parse);
  std::string parse_given_extension(std::string to_parse);

  std::string get_format(std::string media_url);
  std::string get_extension(std::string media_url);

  
};

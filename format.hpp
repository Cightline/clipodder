
#include <string>
#include <vector>

#include "debug.hpp"

class format
{
  
  debug dbg;

public:
  
  int get_filename(std::string url, std::string *return_url);
  bool defined_type(std::vector<std::string> f_vector, std::string extension, std::string format);
  int parse_given_format(std::string to_parse, std::string *format, std::string *extension);
  std::string determine_format(std::string media_url);
  std::string get_extension(std::string media_url);

  
};

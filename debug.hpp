

#include <iostream>
#include <sstream>

#ifndef DEBUG_HPP
#define DEBUG_HPP


class debug
{



public:
  static int use_debug;
  int current_state();
  void set_state(int state);

};



#endif



#include "debug.hpp"



  
#ifndef DEBUG
#define DEBUG

/* All this static shit is confusing */

/* Define it, outside the class, after its been declared in the class */
int debug::use_debug = 0;

void debug::set_state(int state)
{
  use_debug = state;
}

int debug::state()
{
  return use_debug;
}


#endif

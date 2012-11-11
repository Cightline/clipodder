#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <stdarg.h>

class output
{
    public:
        static int verbose;
        static int warnings; 
        static int suppress; 
        static void msg(int level, const char *message, ...);


};

#endif

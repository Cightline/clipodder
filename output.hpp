#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>


class output
{
    public:
        static int verbose;
        static int warnings; 
        static int suppress; 
        static void msg(std::string message, int level, bool newline=true);
        static void num(int number, int level, bool newline=false);


};

#endif

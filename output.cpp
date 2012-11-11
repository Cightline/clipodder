
#include "output.hpp"
#include <stdio.h>


void output::msg(int level, const char *message, ...)
{
    /* 1 == verbose
       2 == warning
       3 == error
    */ 

    /* If it does not match, skip else print */

    if (level == 1 and output::verbose != 1)
    {
        return;
    }

    else if (level == 2 and output::warnings != 1)
    {
        return; 
    }

    char buffer[256];
    va_list args;

    va_start(args, message);
    vsnprintf(buffer, sizeof(buffer), message, args);

    va_end(args);
    
    std::cout << buffer << std::endl;
    
}


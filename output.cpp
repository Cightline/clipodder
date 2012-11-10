
#include "output.hpp"
#include <sstream>

void output::num(int number, int level, bool newline)
{
    std::stringstream converted;
    converted << number;
    output::msg(converted.str(), level, newline);
}
    
void output::msg(std::string message, int level, bool newline)
{
    /* 1 == verbose
       2 == warning
       3 == error
    */ 

    bool did_cout = false;
    /* verbose */
    if (verbose == 1 && level == 1)
    {
        std::cout << message;
        did_cout = true;
    }

    /* warnings */
    else if (warnings == 1 && level == 2)
    {
        std::cout << message;
        did_cout = true;
    }

    /* quiet */
    else if (suppress == 0 && level == 0)
    {
        std::cout << message;
        did_cout = true;
    }
    
    if (newline == true && did_cout == true)
    {
        std::cout << std::endl;
    }
}


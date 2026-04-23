#include <iostream>

#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
    try
    {
        Bureaucrat chief("Chief", 1);
        Form permit("TopSecret", 10, 10);
        std::cout << chief << std::endl;
        std::cout << permit << std::endl;
        chief.signForm(permit);
        std::cout << permit << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "caught: " << e.what() << std::endl;
    }
    return 0;
}

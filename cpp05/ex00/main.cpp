#include <iostream>

#include "Bureaucrat.hpp"

int main() 
{
    try {
        Bureaucrat bob("Bob", 1);
        // Bureaucrat alice("Alice", 150);
        std::cout << bob << std::endl;
        bob.incrementGrade();
        
        // bob.incrementGrade();
    //     bob.incrementGrade();

        // std::cout << bob << std::endl;
    } catch (const std::exception& a) 
    {
        std::cout << "caught: " << a.what() << std::endl;
    }

    return 0;
}

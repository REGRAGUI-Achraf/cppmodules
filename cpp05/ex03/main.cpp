#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

int main()
{
    // Créons un stagiaire
    Intern intern;
    Bureaucrat chief("Chief", 1);
    Bureaucrat manager("Manager", 70);
    Bureaucrat internBureaucrat("Intern", 150);

    AForm* form;

    std::cout << "--- Test 1: Intern creates a ShrubberyCreationForm ---" << std::endl;
    try
    {
        form = intern.makeForm("shrubbery creation", "garden");
        if (form)
        {
            std::cout << "Intern created " << form->getName() << std::endl;
            manager.signForm(*form);
            chief.executeForm(*form);
            delete form;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    std::cout << "--- Test 2: Intern creates a RobotomyRequestForm ---" << std::endl;
    try
    {
        form = intern.makeForm("robotomy request", "Bender");
        if (form)
        {
            std::cout << "Intern created " << form->getName() << std::endl;
            manager.signForm(*form);
            chief.executeForm(*form);
            delete form;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    std::cout << "--- Test 3: Intern creates a PresidentialPardonForm ---" << std::endl;
    try
    {
        form = intern.makeForm("presidential pardon", "Ford");
        if (form)
        {
            std::cout << "Intern created " << form->getName() << std::endl;
            manager.signForm(*form);
            chief.executeForm(*form);
            delete form;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    std::cout << "--- Test 4: Intern fails to create an unknown form ---" << std::endl;
    try
    {
        form = intern.makeForm("invalid form type", "target");
        if (!form)
        {
            std::cout << "Intern could not create the form: type unknown." << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Intern could not create form because: " << e.what() << '\n';
    }

    return 0;
}
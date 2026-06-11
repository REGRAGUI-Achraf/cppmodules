#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

int main() {
    Bureaucrat chief("Chief", 1);
    Bureaucrat manager("Manager", 70);
    Bureaucrat intern("Intern", 150);

    ShrubberyCreationForm shrub("home");
    RobotomyRequestForm robot("Bender");
    PresidentialPardonForm pardon("Arthur");

    std::cout << shrub << std::endl;
    std::cout << robot << std::endl;
    std::cout << pardon << std::endl;

    intern.signForm(shrub);
    chief.executeForm(shrub);

    manager.signForm(shrub);
    chief.executeForm(shrub);

    manager.signForm(robot);
    manager.executeForm(robot);
    chief.executeForm(robot);

    manager.signForm(pardon);
    chief.signForm(pardon);
    chief.executeForm(pardon);

    return 0;
}

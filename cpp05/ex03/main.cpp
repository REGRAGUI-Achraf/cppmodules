#include "Bureaucrat.hpp"
#include "Intern.hpp"

#include <iostream>

int main() {
    Intern intern;
    Bureaucrat chief("Chief", 1);
    Bureaucrat manager("Manager", 70);
    Bureaucrat internBureaucrat("Intern", 150);

    AForm* rrf = intern.makeForm("robotomy request", "Bender");
    AForm* scf = intern.makeForm("shrubbery creation", "home");
    AForm* ppf = intern.makeForm("presidential pardon", "Arthur");
    AForm* unknown = intern.makeForm("unknown", "nowhere");

    if (rrf) {
        manager.signForm(*rrf);
        chief.executeForm(*rrf);
    }

    if (scf) {
        internBureaucrat.signForm(*scf);
        chief.executeForm(*scf);
    }

    if (ppf) {
        chief.signForm(*ppf);
        chief.executeForm(*ppf);
    }

    delete rrf;
    delete scf;
    delete ppf;
    delete unknown;

    return 0;
}
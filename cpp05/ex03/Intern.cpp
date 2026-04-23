#include "Intern.hpp"

#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

#include <cstddef>
#include <iostream>

namespace {
AForm* createShrubbery(const std::string& target) {
    return new ShrubberyCreationForm(target);
}

AForm* createRobotomy(const std::string& target) {
    return new RobotomyRequestForm(target);
}

AForm* createPresidential(const std::string& target) {
    return new PresidentialPardonForm(target);
}

struct FormEntry {
    const char* name;
    AForm* (*create)(const std::string& target);
};
}

Intern::Intern() {}

Intern::Intern(const Intern& other) {
    (void)other;
}

Intern& Intern::operator=(const Intern& other) {
    (void)other;
    return *this;
}

Intern::~Intern() {}

AForm* Intern::makeForm(const std::string& name, const std::string& target) const {
    static const FormEntry forms[] = {
        {"shrubbery creation", &createShrubbery},
        {"robotomy request", &createRobotomy},
        {"presidential pardon", &createPresidential}
    };

    for (size_t i = 0; i < sizeof(forms) / sizeof(forms[0]); ++i) {
        if (name == forms[i].name) {
            AForm* form = forms[i].create(target);
            std::cout << "Intern creates " << name << std::endl;
            return form;
        }
    }

    std::cout << "Intern couldn't create form \"" << name << "\"" << std::endl;
    return 0;
}
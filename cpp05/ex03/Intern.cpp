#include "Intern.hpp"

static AForm* createShrubbery(const std::string& target) {
    return new ShrubberyCreationForm(target);
}

static AForm* createRobotomy(const std::string& target) {
    return new RobotomyRequestForm(target);
}

static AForm* createPresidential(const std::string& target) {
    return new PresidentialPardonForm(target);
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
    static const char* formNames[] = {
        "shrubbery creation",
        "robotomy request",
        "presidential pardon"
    };
    static AForm* (*creators[])(const std::string& target) = {
        &createShrubbery,
        &createRobotomy,
        &createPresidential
    };

    size_t i = 0;
    while (i < sizeof(formNames) / sizeof(formNames[0])) {
        if (name == formNames[i]) {
            AForm* form = creators[i](target);
            std::cout << "Intern creates " << name << std::endl;
            return form;
        }
        ++i;
    }

    std::cout << "Intern couldn't create form \"" << name << "\"" << std::endl;
    return 0;
}
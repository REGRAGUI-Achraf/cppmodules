#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "Bureaucrat.hpp"
#include "AForm.hpp"

#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

class RobotomyRequestForm : public AForm {
public:
    RobotomyRequestForm();
    RobotomyRequestForm(const std::string& target);
    RobotomyRequestForm(const RobotomyRequestForm& other);
    RobotomyRequestForm& operator=(const RobotomyRequestForm& other);
    ~RobotomyRequestForm();

    void execute(Bureaucrat const& executor) const;
    const std::string& getTarget() const;

private:
    std::string target;
};

#endif

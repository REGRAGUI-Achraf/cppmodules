#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat() : name(" "), grade(150) {}

Bureaucrat::Bureaucrat(const std::string& name, int grade) : name(name), grade(grade) {
    if (grade < 1) {
        throw GradeTooHighException();
    }
    if (grade > 150) {
        throw GradeTooLowException();
    }
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : name(other.name), grade(other.grade) {}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other) {
    if (this != &other) {
        grade = other.grade;
    }
    return *this;
}

Bureaucrat::~Bureaucrat() {}

const std::string& Bureaucrat::getName() const {
    return name;
}

int Bureaucrat::getGrade() const {
    return grade;
}

void Bureaucrat::incrementGrade() {
    if (grade - 1 < 1) {
        throw GradeTooHighException();
    }
    --grade;
}

void Bureaucrat::decrementGrade() {
    if (grade + 1 > 150) {
        throw GradeTooLowException();
    }
    ++grade;
}


const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "Bureaucrat grade too high";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "Bureaucrat grade too low";
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat) {
    os << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade();
    return os;
}

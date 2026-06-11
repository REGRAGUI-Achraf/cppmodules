#include "Form.hpp"

Form::Form() : name("Form"), signed_(false), gradeToSign(150), gradeToExecute(150) {}

Form::Form(const std::string& name, int gradeToSign, int gradeToExecute) 
    : name(name), signed_(false), gradeToSign(gradeToSign), gradeToExecute(gradeToExecute)
{
    if (gradeToSign < 1 || gradeToExecute < 1) {
        throw GradeTooHighException();
    }
    if (gradeToSign > 150 || gradeToExecute > 150) {
        throw GradeTooLowException();
    }
}

Form::Form(const Form& other)
    : name(other.name), signed_(other.signed_), gradeToSign(other.gradeToSign), gradeToExecute(other.gradeToExecute) {}

Form& Form::operator=(const Form& other) {
    if (this != &other) {
        signed_ = other.signed_;
    }
    return *this;
}

Form::~Form() {}

const std::string& Form::getName() const {
    return name;
}

bool Form::isSigned() const {
    return signed_;
}

int Form::getGradeToSign() const {
    return gradeToSign;
}

int Form::getGradeToExecute() const {
    return gradeToExecute;
}

void Form::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > gradeToSign) {
        throw GradeTooLowException();
    }
    signed_ = true;
}

const char* Form::GradeTooHighException::what() const throw() {
    return "Form grade too high";
}

const char* Form::GradeTooLowException::what() const throw() {
    return "Form grade too low";
}

std::ostream& operator<<(std::ostream& os, const Form& form) {
    os << "Form " << form.getName() << ", signed " << (form.isSigned() ? "true" : "false")
       << ", grade to sign " << form.getGradeToSign() << ", grade to execute "
       << form.getGradeToExecute() << ".";
    return os;
}

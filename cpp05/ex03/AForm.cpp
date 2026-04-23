#include "AForm.hpp"

#include "Bureaucrat.hpp"

#include <ostream>

AForm::AForm() : name("AForm"), signed_(false), gradeToSign(150), gradeToExecute(150) {}

AForm::AForm(const std::string& name, int gradeToSign, int gradeToExecute)
    : name(name), signed_(false), gradeToSign(gradeToSign), gradeToExecute(gradeToExecute)
{
    if (gradeToSign < 1 || gradeToExecute < 1) {
        throw GradeTooHighException();
    }
    if (gradeToSign > 150 || gradeToExecute > 150) {
        throw GradeTooLowException();
    }
}

AForm::AForm(const AForm& other)
    : name(other.name), signed_(other.signed_), gradeToSign(other.gradeToSign),
      gradeToExecute(other.gradeToExecute) {}

AForm& AForm::operator=(const AForm& other) {
    if (this != &other) {
        signed_ = other.signed_;
    }
    return *this;
}

AForm::~AForm() {}

const std::string& AForm::getName() const {
    return name;
}

bool AForm::isSigned() const {
    return signed_;
}

int AForm::getGradeToSign() const {
    return gradeToSign;
}

int AForm::getGradeToExecute() const {
    return gradeToExecute;
}

void AForm::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > gradeToSign) {
        throw GradeTooLowException();
    }
    signed_ = true;
}

void AForm::assertExecutable(Bureaucrat const& executor) const {
    if (!signed_) {
        throw FormNotSignedException();
    }
    if (executor.getGrade() > gradeToExecute) {
        throw GradeTooLowException();
    }
}

const char* AForm::GradeTooHighException::what() const throw() {
    return "Form grade too high";
}

const char* AForm::GradeTooLowException::what() const throw() {
    return "Form grade too low";
}

const char* AForm::FormNotSignedException::what() const throw() {
    return "Form not signed";
}

std::ostream& operator<<(std::ostream& os, const AForm& form) {
    os << "Form " << form.getName() << ", signed " << (form.isSigned() ? "true" : "false")
       << ", grade to sign " << form.getGradeToSign() << ", grade to execute "
       << form.getGradeToExecute() << ".";
    return os;
}
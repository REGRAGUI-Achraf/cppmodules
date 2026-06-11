#ifndef AFORM_HPP
#define AFORM_HPP

#include <exception>
#include <iosfwd>
#include <string>
#include <ostream>
#include "Bureaucrat.hpp"

class Bureaucrat;

class AForm 
{
public:
	AForm();
	AForm(const std::string& name, int gradeToSign, int gradeToExecute);
	AForm(const AForm& other);
	AForm& operator=(const AForm& other);
	virtual ~AForm();

	const std::string& getName() const;
	bool isSigned() const;
	int getGradeToSign() const;
	int getGradeToExecute() const;

	void beSigned(const Bureaucrat& bureaucrat);
	virtual void execute(Bureaucrat const& executor) const = 0;

	class GradeTooHighException : public std::exception {
	public:
		const char* what() const throw();
	};

	class GradeTooLowException : public std::exception {
	public:
		const char* what() const throw();
	};

	class FormNotSignedException : public std::exception {
	public:
		const char* what() const throw();
	};

private:
	const std::string name;
	bool signed_;
	const int gradeToSign;
	const int gradeToExecute;
};

std::ostream& operator<<(std::ostream& os, const AForm& form);

#endif

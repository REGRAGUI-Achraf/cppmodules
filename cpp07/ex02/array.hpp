#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <exception>

template <typename T>
class Array
{
	public:
		Array();
		Array(unsigned int n);
		Array(const Array &src);
		Array &operator=(const Array &rhs);
		~Array();

		T &operator[](unsigned int index);
		const T &operator[](unsigned int index) const;

		unsigned int size() const;

		class OutOfBoundsException : public std::exception
		{
			public:
				const char *what() const throw();
		};

	private:
		T			*data;
		unsigned int	length;
};

#include "array.tpp"

#endif
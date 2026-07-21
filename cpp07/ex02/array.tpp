#ifndef ARRAY_TPP
#define ARRAY_TPP

#include "array.hpp"

// tableau vide
template <typename T>
Array<T>::Array() : data(NULL), length(0)
{
}

template <typename T>
Array<T>::Array(unsigned int n) : data(NULL), length(n)
{
    if (length > 0)
    data = new T[length]();
}

template <typename T>
Array<T>::Array(const Array &src) : data(NULL), length(0)
{
	*this = src;
}

// Operateur d'affectation : copie profonde, libere l'ancienne memoire
template <typename T>
Array<T> &Array<T>::operator=(const Array &src)
{
	if (this == &src)
		return (*this);

	delete[] data;
	data = NULL;
	length = src.length;

	if (length > 0)
	{
		data = new T[length];
		for (unsigned int i = 0; i < length; ++i)
			data[i] = src.data[i];
	}
	return (*this);
}

// Destructeur : libere la memoire allouee avec new[]
template <typename T>
Array<T>::~Array()
{
	delete[] data;
}

// Operateur [] non-const : verifie les bornes, sinon exception
template <typename T>
T &Array<T>::operator[](unsigned int index)
{
	if (index >= length)
		throw OutOfBoundsException();
	return (data[index]);
}

// Operateur [] const : meme verification
template <typename T>
const T &Array<T>::operator[](unsigned int index) const
{
	if (index >= length)
		throw OutOfBoundsException();
	return (data[index]);
}

// size() : ne modifie pas l'instance (const)
template <typename T>
unsigned int Array<T>::size() const
{
	return (length);
}

// Message de l'exception
template <typename T>
const char *Array<T>::OutOfBoundsException::what() const throw()
{
	return ("Array: index out of bounds");
}

#endif
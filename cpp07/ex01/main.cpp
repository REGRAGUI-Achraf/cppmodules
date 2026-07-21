#include <iostream>
#include <string>
#include "iter.hpp"

template <typename T>
void afficher(T &x)
{
	std::cout << x << std::endl;
}

template <typename T>
void afficherConst(const T &x)
{
	std::cout << x << std::endl;
}

void modifier(int &x)
{
	x *= 2;
}

int main(void)
{
	int tab[] = {1, 2, 3, 4, 5};
	size_t tabLen = sizeof(tab) / sizeof(tab[0]);

	std::cout << "doubler les entiers en place " << std::endl;
	iter(tab, tabLen, modifier);
	iter(tab, tabLen, afficher<int>);

	std::cout << "afficher via reference const " << std::endl;
	iter(tab, tabLen, afficherConst<int>);

	const int constTab[] = {10, 20, 30};
	size_t constTabLen = sizeof(constTab) / sizeof(constTab[0]);

	std::cout << "afficher un tableau const" << std::endl;
	iter(constTab, constTabLen, afficherConst<int>);

	std::string strs[] = {"Hello", "42", "iter"};
	size_t strsLen = sizeof(strs) / sizeof(strs[0]);

	std::cout << "-- afficher des strings --" << std::endl;
	iter(strs, strsLen, afficher<std::string>);

	return 0;
}
#include <iostream>
#include <string>
#include "array.hpp"

int main(void)
{
	// 1. Construction sans parametre : tableau vide
	std::cout << "-- tableau vide --" << std::endl;
	Array<int> empty;
	std::cout << "taille: " << empty.size() << std::endl;

	// 2. Construction avec n : elements initialises par defaut
	std::cout << "-- tableau de 5 int par defaut --" << std::endl;
	Array<int> tab(5);
	std::cout << "taille: " << tab.size() << std::endl;
	for (unsigned int i = 0; i < tab.size(); ++i)
		std::cout << "tab[" << i << "] = " << tab[i] << std::endl; // doit afficher 0

	// tip du sujet : new int() initialise a 0
	int *a = new int();
	std::cout << "int* a = new int() -> *a = " << *a << std::endl;
	delete a;

	// 3. Remplissage du tableau
	std::cout << "-- remplissage --" << std::endl;
	for (unsigned int i = 0; i < tab.size(); ++i)
		tab[i] = (i + 1) * 10;
	for (unsigned int i = 0; i < tab.size(); ++i)
		std::cout << "tab[" << i << "] = " << tab[i] << std::endl;

	// 4. Constructeur par copie : deep copy
	std::cout << "-- constructeur par copie --" << std::endl;
	Array<int> copyTab(tab);
	copyTab[0] = 999;
	std::cout << "tab[0] = " << tab[0] << " (doit rester 10)" << std::endl;
	std::cout << "copyTab[0] = " << copyTab[0] << " (doit etre 999)" << std::endl;

	// 5. Operateur d'affectation : deep copy
	std::cout << "-- operateur d'affectation --" << std::endl;
	Array<int> assignedTab;
	assignedTab = tab;
	assignedTab[1] = 777;
	std::cout << "tab[1] = " << tab[1] << " (doit rester 20)" << std::endl;
	std::cout << "assignedTab[1] = " << assignedTab[1] << " (doit etre 777)" << std::endl;

	// 6. Acces hors bornes -> exception
	std::cout << "-- acces hors bornes --" << std::endl;
	try
	{
		std::cout << tab[100] << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception attrapee: " << e.what() << std::endl;
	}

	// aussi sur la version const de l'operateur []
	const Array<int> constTab(tab);
	try
	{
		std::cout << constTab[50] << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception attrapee (const): " << e.what() << std::endl;
	}

	// 7. Fonctionne avec n'importe quel type (ex: std::string)
	std::cout << "-- tableau de string --" << std::endl;
	Array<std::string> strArr(3);
	strArr[0] = "Hello";
	strArr[1] = "42";
	strArr[2] = "Array";
	for (unsigned int i = 0; i < strArr.size(); ++i)
		std::cout << strArr[i] << std::endl;

	// 8. size() ne modifie pas l'instance : marche sur un objet const
	std::cout << "-- size() sur const --" << std::endl;
	const Array<int> constArr(4);
	std::cout << "taille constArr: " << constArr.size() << std::endl;

	return 0;
}
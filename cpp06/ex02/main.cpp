#include "funcs.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // srand() est appele UNE SEULE FOIS, ici, au debut du programme.
    srand(static_cast<unsigned int>(time(NULL)));

    for (int i = 0; i < 10; i++) {
        Base* p = generate();

        std::cout << "identify(Base*): ";
        identify(p);

        std::cout << "identify(Base&): ";
        identify(*p);

        delete p;
        std::cout << "------------------------" << std::endl;
    }

    return 0;
}

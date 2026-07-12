#include <iostream>
#include <algorithm>

class bigint {
    public:
        // -------------------------------------------------------
        // BLOC 1 : L'État et les Utilitaires (Les "Helpers")
        // -------------------------------------------------------
        std::string value;
        
        void remove_zeros();
        std::string addStrings(const std::string &a, const std::string &b) const;
        unsigned long long stringToULL(const std::string &s) const;

        // -------------------------------------------------------
        // BLOC 2 : Forme Canonique & Constructeurs
        // -------------------------------------------------------
        bigint();                                     // Défaut
        bigint(const std::string& value);             // Paramétrique (String)
        bigint(unsigned long long n);                 // Paramétrique (ULL)
        
        bigint(const bigint& obj);                    // Copie
        bigint& operator=(const bigint& obj);         // Assignation

        // -------------------------------------------------------
        // BLOC 3 : Les Comparaisons (Toujours bool et const)
        // -------------------------------------------------------
        bool operator==(const bigint& obj) const;
        bool operator!=(const bigint& obj) const;
        bool operator<(const bigint& obj) const;
        bool operator>(const bigint& obj) const;
        bool operator<=(const bigint& obj) const;
        bool operator>=(const bigint& obj) const;

        // -------------------------------------------------------
        // BLOC 4 : L'Arithmétique (+, +=, ++)
        // -------------------------------------------------------
        // Avec un autre bigint
        bigint  operator+(const bigint& obj) const;
        bigint& operator+=(const bigint& obj);
        
        // Avec un unsigned long long
        bigint  operator+(unsigned long long n) const;
        bigint& operator+=(unsigned long long n);
        
        // Incrémentation
        bigint& operator++();    // Pré-incrémentation (++obj)
        bigint  operator++(int); // Post-incrémentation (obj++)

        // -------------------------------------------------------
        // BLOC 5 : Les Décalages (Shifts : <<, >>, <<=, >>=)
        // -------------------------------------------------------
        // Version avec un entier classique (int)
        bigint  operator<<(int shift) const;
        bigint  operator>>(int shift) const;
        bigint& operator<<=(int shift);
        bigint& operator>>=(int shift);
        
        // Version avec un objet bigint
        bigint  operator<<(const bigint& shift) const;
        bigint  operator>>(const bigint& shift) const;
        bigint& operator<<=(const bigint& shift);
        bigint& operator>>=(const bigint& shift);
};

// -------------------------------------------------------
// BLOC 6 : Flux d'entrée/sortie (Hors de la classe !)
// -------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const bigint& obj);
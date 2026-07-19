# Pointeur vs Référence

## Tableau comparatif

| Aspect | Pointeur | Référence |
|--------|----------|-----------|
| **Peut être nul** | Oui | Non |
| **Peut changer de cible** | Oui | Non |
| **Doit être initialisé** | Non | Oui, dès la création |
| **Syntaxe pour accéder** | `*p` ou `p->` | `r` directement |
| **Syntaxe pour l'adresse** | `&variable` | `&variable` |
| **Peut être passé sans cible** | Oui | Non |

## Exemples simples

### Pointeur

```cpp
int a = 10;
int* p = &a;           // p contient l'adresse de a
*p = 20;               // change la valeur de a via p
std::cout << *p;       // affiche 20

p = nullptr;           // un pointeur peut être nul
if (p) {
    std::cout << *p;   // n'affiche rien car p est nul
}

int b = 30;
p = &b;                // p peut pointer vers un autre objet
std::cout << *p;       // affiche 30
```

### Référence

```cpp
int a = 10;
int& r = a;            // r est un alias de a
r = 20;                // change la valeur de a
std::cout << r;        // affiche 20

// r = nullptr;        // ERREUR : impossible, une ref ne peut pas être nulle

int b = 30;
// r = b;              // ERREUR : impossible, r reste toujours liée à a
```

## Cas d'usage

### Utiliser un pointeur quand :
- l'objet peut être `nullptr`
- tu veux changer la cible
- tu veux la stocker dans une collection

```cpp
Base* p = nullptr;
if (condition)
    p = new A();
```

### Utiliser une référence quand :
- tu veux un alias permanent
- tu veux passer un objet à une fonction sans risque qu'il soit nul
- tu veux un code plus lisible

```cpp
void modify(int& x) {
    x = 100;  // c'est clair qu'on modifie l'argument
}

int a = 10;
modify(a);  // pas besoin de & ou * dans l'appel
```

## Dans ton exercice (ex02)

La différence de comportement avec `dynamic_cast` :

### Avec pointeur

```cpp
void identify(Base* p) {
    if (!p) return;
    if (dynamic_cast<A*>(p))      // retourne A* ou nullptr
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
}
```

C'est simple : vérifie `nullptr`.

### Avec référence

```cpp
void identify(Base& p) {
    try {
        dynamic_cast<A&>(p);      // lance exception si pas un A
        std::cout << "A" << std::endl;
    } catch (...) {}              // attrape l'exception
    
    try {
        dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
    } catch (...) {}
}
```

Plus compliqué : il faut gérer les exceptions.

## Résumé très basique

- **Pointeur** = une flèche qu'on peut diriger où on veut
- **Référence** = un alias qu'on ne peut pas changer

Comme une URL vs un raccourci de fichier :
- URL (pointeur) : on peut changer vers un autre site
- raccourci (référence) : pointe toujours au même endroit

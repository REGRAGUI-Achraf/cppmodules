# Gestion des exceptions en C++

## Qu'est-ce qu'une exception ?

Une exception c'est un signal d'erreur que le programme lance quand quelque chose de grave se produit. Au lieu de s'arrêter brutalement, le programme peut l'attraper et continuer.

## Syntaxe de base

### Lancer une exception (throw)

```cpp
if (valeur < 0) {
    throw std::runtime_error("La valeur ne peut pas être négative");
}
```

### Attraper une exception (try/catch)

```cpp
try {
    // Code qui peut lancer une exception
    if (valeur < 0)
        throw std::runtime_error("Erreur");
}
catch (std::runtime_error& e) {
    // Code exécuté si l'exception est attrapée
    std::cout << "Erreur attrapée : " << e.what() << std::endl;
}
```

## Exemple simple

Sans gestion d'exception :
```cpp
int diviser(int a, int b) {
    return a / b;  // Crash si b == 0
}
```

Avec gestion d'exception :
```cpp
int diviser(int a, int b) {
    if (b == 0)
        throw std::invalid_argument("Division par zéro");
    return a / b;
}

int main() {
    try {
        int resultat = diviser(10, 0);
    }
    catch (std::invalid_argument& e) {
        std::cout << "Erreur : " << e.what() << std::endl;
    }
    return 0;
}
```

## dynamic_cast avec une référence

Quand tu fais `dynamic_cast` sur une référence et que ça échoue, il lance une exception de type `std::bad_cast`.

### Cas qui échoue

```cpp
Base& ref = *p;  // p pointe vers un B

try {
    A& a = dynamic_cast<A&>(ref);  // ERREUR : ce n'est pas un A
    std::cout << "C'est un A" << std::endl;
}
catch (std::bad_cast& e) {
    std::cout << "Ce n'est pas un A" << std::endl;
}
```

### Cas qui marche

```cpp
Base& ref = *p;  // p pointe vers un A

try {
    A& a = dynamic_cast<A&>(ref);  // OK : c'est un A
    std::cout << "C'est un A" << std::endl;
}
catch (std::bad_cast& e) {
    std::cout << "Ce n'est pas un A" << std::endl;
}
```

## Dans ton exercice 02

La fonction `identify(Base& p)` fait plusieurs tests en cascade :

```cpp
void identify(Base& p) {
    // Test 1 : est-ce un A ?
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    } catch (...) {}  // On ignore l'erreur et on continue
    
    // Test 2 : est-ce un B ?
    try {
        (void)dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
        return;
    } catch (...) {}  // On ignore l'erreur et on continue
    
    // Test 3 : est-ce un C ?
    try {
        (void)dynamic_cast<C&>(p);
        std::cout << "C" << std::endl;
        return;
    } catch (...) {}  // On ignore l'erreur et on continue
}
```

La ligne `catch (...)` signifie "attrape n'importe quelle exception".

## Points importants

### Les trois types de base

```cpp
std::invalid_argument   // Argument invalide
std::runtime_error      // Erreur à l'exécution
std::bad_cast           // Échec de dynamic_cast (référence)
```

### Toujours utiliser try/catch pour

- `dynamic_cast` sur une référence
- Opérations sur des fichiers
- Allocations mémoire
- Conversions de types risquées

### Ne pas attraper les exceptions si

- Tu ne sais pas quoi faire
- L'erreur doit remonter au programme principal

## Comparaison : pointeur vs référence

### Avec pointeur (pas d'exception)

```cpp
Base* p = new B();

if (dynamic_cast<A*>(p)) {
    std::cout << "A" << std::endl;
} else if (dynamic_cast<B*>(p)) {
    std::cout << "B" << std::endl;
}

delete p;
```

### Avec référence (exception)

```cpp
Base* p = new B();
Base& ref = *p;

try {
    dynamic_cast<A&>(ref);
    std::cout << "A" << std::endl;
} catch (std::bad_cast&) {
    try {
        dynamic_cast<B&>(ref);
        std::cout << "B" << std::endl;
    } catch (std::bad_cast&) {}
}

delete p;
```

## Résumé

- **try** : bloc de code à surveiller
- **catch** : bloc exécuté si exception
- **throw** : lancer une exception
- **catch (...)** : attrape toute exception
- Pour `dynamic_cast<T&>`, toujours utiliser try/catch car ça lance `std::bad_cast` en cas d'échec

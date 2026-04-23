# Module 05 - ex00 : "Maman, quand je serai grand, je veux être bureaucrate !"

Cet exercice introduit la gestion des exceptions en C++98 en construisant une classe de base `Bureaucrat`. Voici une explication claire, étape par étape, des concepts avec des exemples.

## 1. Ce que l'exercice vous demande de construire

Vous devez construire un système qui représente un `Bureaucrat` travaillant dans une administration.
Chaque bureaucrate possède un **nom** et un **échelon (grade)**.
La seule subtilité réside dans le système de notation : **Le grade 1 est le plus élevé** (niveau PDG), et **le grade 150 est le plus bas** (niveau stagiaire). Vous devez vous assurer que personne ne puisse jamais exister en dehors de cette plage de 1 à 150.

## 2. Le rôle de chaque exigence

- **Nom constant (`const std::string name`)** : Un bureaucrate ne peut pas changer d'identité une fois embauché. Comme il est `const`, vous DEVEZ l'initialiser dans la liste d'initialisation du constructeur, et non à l'intérieur du corps `{}` du constructeur.
- **Grade entre 1 et 150** : Ce sont les limites physiques de votre administration. 1 est le meilleur, 150 le pire. Toute tentative de sortir de ces limites doit immédiatement faire planter l'action proprement.
- **Fonctions d'incrémentation / décrémentation** :
  - *Incrémenter* un grade signifie promouvoir le bureaucrate. Comme 1 est le plus haut, "incrémenter" signifie en réalité **soustraire 1** à son grade actuel. (Le grade 3 devient le grade 2).
  - *Décrémenter* signifie le rétrograder, ce qui implique d'**ajouter 1**. (Le grade 3 devient le grade 4).
- **Exceptions (`GradeTooHighException`, `GradeTooLowException`)** : Ce sont des types d'erreurs personnalisés. Si quelqu'un essaie de créer un Bureaucrate avec un grade de 0, ou rétrograde un grade 150 en 151, ces exceptions sont déclenchées (lancées) pour signaler fortement l'erreur.

## 3. Pourquoi les exceptions ? Comment fonctionnent `try/catch` ici ?

Avant les exceptions, les fonctions renvoyaient des codes d'erreur (comme `-1` ou `NULL`). Mais **les constructeurs n'ont pas de valeur de retour**. Si quelqu'un écrit `Bureaucrat b("Bob", 200);`, comment le constructeur peut-il dire "J'ai échoué" ?
Il ne peut pas renvoyer `-1`. Au lieu de cela, il **lance (throw) une exception**.

Lancer une exception arrête immédiatement l'exécution du bloc en cours.
- **Bloc `try`** : Vous y placez votre code risqué. ("Essaie de faire ça...").
- **Bloc `catch`** : Si une erreur est lancée dans le bloc `try`, le programme saute immédiatement au bloc `catch` pour la gérer en toute sécurité, évitant ainsi un plantage complet du programme.

**Exemple :**
```cpp
try {
    // Action risquée
    Bureaucrat bob("Bob", 151); // Ceci va lancer une exception !
    std::cout << "Cette ligne ne s'affichera jamais.";
} catch (std::exception& e) {
    // Atterrissage en toute sécurité
    std::cout << "Erreur attrapée : " << e.what() << std::endl;
}
```

## 4. Qu'est-ce que la "Forme Canonique Orthodoxe" (Orthodox Canonical Form) ?

En C++98 (et particulièrement à l'école 42), la Forme Canonique Orthodoxe est une règle stipulant que votre classe doit fournir explicitement 4 éléments :
1. **Constructeur par défaut** (`Bureaucrat()`)
2. **Constructeur de copie** (`Bureaucrat(const Bureaucrat& other)`)
3. **Opérateur d'assignation** (`Bureaucrat& operator=(const Bureaucrat& other)`)
4. **Destructeur** (`~Bureaucrat()`)

**Pourquoi est-ce requis ?**
Lorsque vous passez des objets à des fonctions, les copiez ou les détruisez, le C++ génère des comportements par défaut. Parfois, ces comportements par défaut provoquent des bugs, des fuites de mémoire ou des "copies superficielles" (deux objets pointant vers la même mémoire). Vous obliger à écrire la Forme Canonique garantit que vous contrôlez toujours manuellement la façon exacte dont vos objets sont dupliqués et détruits.

## 5. Surcharge de l'opérateur `operator<<`

Quand vous voulez afficher un entier, vous faites `std::cout << monEntier`.
Quand vous faites `std::cout << monBureaucrate`, le compilateur n'a aucune idée de la façon de convertir un `Bureaucrat` en texte.

En surchargeant `operator<<`, vous apprenez au compilateur comment afficher votre classe en définissant vous même le comportement quand l'objet rencontre un `ostream`.
Au lieu d'obliger les utilisateurs à écrire `bob.print();`, ils peuvent simplement injecter votre objet dans un flux :
`std::cout << "Regardez : " << bob << std::endl;`

## 6. Erreurs courantes des étudiants

1. **Inverser l'incrémentation/décrémentation** : Les étudiants font souvent `grade++` pour incrémenter. Rappelez-vous : le grade 1 est le SOMMET. Promouvoir signifie soustraire !
2. **Oublier de vérifier les limites dans le constructeur** : Vous devez lancer des exceptions si un grade invalide est passé lors de la *création*, pas seulement lors de l'incrémentation/décrémentation.
**Exemple d'erreur dans le constructeur :**
```cpp
Bureaucrat::Bureaucrat(std::string const & name, int grade) : name(name), grade(grade) {
   // ERREUR: il manque un check ici ! Si on cree Bureaucrat("Test", 200) ca passe !
}
```
**Correction :**
```cpp
Bureaucrat::Bureaucrat(std::string const & name, int grade) : name(name) {
    if (grade < 1)
        throw Bureaucrat::GradeTooHighException();
    if (grade > 150)
        throw Bureaucrat::GradeTooLowException();
    this->grade = grade;
}
```
3. **Attraper (catch) par valeur, pas par référence** : Attrapez toujours les exceptions par référence (`catch (std::exception& e)`), et non par valeur (`catch (std::exception e)`). Attraper par valeur crée une nouvelle copie de l'exception (slicing) et vous perdez le vrai type et son message originel.
4. **Erreur d'initialisation de `const`** : Comme `name` est constant, `Bureaucrat::Bureaucrat(std::string name) { this->name = name; }` ne compilera pas. Vous DEVEZ utiliser une liste d'initialisation : `Bureaucrat::Bureaucrat(std::string name) : name(name) {}`.

## 7. Petit exemple de comportement global

Voici comment la classe doit se comporter dans un fichier `main.cpp` :

```cpp
#include <iostream>
#include "Bureaucrat.hpp"

int main() {
    std::cout << "--- Test 1 : Exception de Limites ---" << std::endl;
    try {
        Bureaucrat chief("Chef", 0); // Invalide, les limites sont de 1 à 150
        // L'exécution s'arrête ici et saute au catch
    } catch (std::exception& e) {
        std::cerr << "Erreur attrapée : " << e.what() << std::endl; 
        // Affiche le message de votre méthode what()
    }

    std::cout << "\n--- Test 2 : Erreur de Rétrogradation ---" << std::endl;
    try {
        Bureaucrat intern("Stagiaire", 150);
        std::cout << intern << std::endl; // Utilise Operator<<
        intern.decrementGrade(); // Devient 151 -> Lance une exception !
    } catch (std::exception& e) {
        std::cerr << "Erreur attrapée : " << e.what() << std::endl;
    }

    std::cout << "\n--- Test 3 : Comportement Normal ---" << std::endl;
    try {
        Bureaucrat manager("Manager", 3);
        manager.incrementGrade();
        std::cout << manager << std::endl; // Affiche : "Manager, bureaucrat grade 2."
    } catch (std::exception& e) {
        std::cerr << "Erreur attrapée : " << e.what() << std::endl;
    }

    return 0;
}
```

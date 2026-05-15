# C++ - Exercices 00 & 01: Gestion des Exceptions et Interactions de Classes

Ce guide est destiné aux débutants en C++ pour les aider à comprendre les concepts fondamentaux de la programmation orientée objet, de la gestion des exceptions et des interactions entre les classes, illustrés par les exercices `ex00` et `ex01`.

## ex00: La Classe `Bureaucrat` et les Exceptions

L'exercice 00 introduit la classe `Bureaucrat`, qui sert de base pour comprendre comment créer des classes, gérer leurs attributs et s'assurer que leur état reste valide à l'aide d'exceptions.

### Qu'est-ce qu'un `Bureaucrat` ?

Un `Bureaucrat` est défini par deux attributs :
- `name` (nom) : Un nom constant qui ne peut pas être changé après la création.
- `grade` (grade) : Un entier représentant son niveau hiérarchique.

**Contrainte importante** : Le grade doit toujours être compris entre 1 (le plus haut) et 150 (le plus bas).

### Le Rôle de `throw` : Lancer une Exception

Comment s'assurer que le grade ne sort jamais de la plage autorisée [1, 150] ? C'est là que les exceptions entrent en jeu.

Dans le constructeur du `Bureaucrat`, nous vérifions la valeur du grade :
```cpp
Bureaucrat::Bureaucrat(const std::string& name, int grade) : name(name), grade(grade) {
    if (grade < 1) {
        throw GradeTooHighException(); // Le grade est trop haut (ex: 0, -1)
    }
    if (grade > 150) {
        throw GradeTooLowException(); // Le grade est trop bas (ex: 151)
    }
}
```
- **`throw`** est un mot-clé qui "lance" une exception. C'est comme tirer une sonnette d'alarme pour signaler qu'une erreur inattendue s'est produite.
- `GradeTooHighException` et `GradeTooLowException` sont des classes personnalisées qui héritent de `std::exception`. Elles représentent des types d'erreurs spécifiques.

Lorsque `throw` est appelé, l'exécution normale du code s'arrête immédiatement. Le programme cherche alors un "gestionnaire d'exception" pour traiter l'erreur.

### `try` et `catch` : Gérer l'Erreur

Si une exception peut être lancée, il faut être prêt à l'attraper. Pour cela, on utilise les blocs `try` et `catch`.

```cpp
int main() {
    try {
        // On essaie de créer un Bureaucrat avec un grade invalide.
        Bureaucrat bob("Bob", 0); 
        std::cout << "Ce message ne s'affichera jamais." << std::endl;
    } catch (const std::exception& e) {
        // Si une exception est lancée dans le bloc 'try', ce bloc 'catch' l'attrape.
        std::cout << "Erreur attrapée: " << e.what() << std::endl;
    }
    return 0;
}
```
- **`try`** : Le code susceptible de lancer une exception est placé dans ce bloc.
- **`catch`** : Si une exception est lancée, le programme saute directement au bloc `catch` correspondant. `e.what()` est une méthode qui retourne un message décrivant l'erreur.

**En résumé pour `ex00`** :
- On crée une classe `Bureaucrat` avec une règle stricte pour son grade.
- On utilise `throw` pour signaler une violation de cette règle.
- On utilise `try` et `catch` pour gérer l'erreur proprement et empêcher le programme de planter.

---

## ex01: Interaction entre `Bureaucrat` et `Form`

L'exercice 01 complexifie les choses en introduisant une nouvelle classe, `Form`, et en faisant interagir `Bureaucrat` avec elle.

### La Classe `Form`

Un `Form` (formulaire) a les attributs suivants :
- `name` : Le nom du formulaire.
- `signed` : Un booléen qui indique si le formulaire est signé ou non.
- `gradeToSign` : Le grade minimum requis pour qu'un `Bureaucrat` puisse le signer.
- `gradeToExecute` : Le grade minimum requis pour exécuter le formulaire.

### L'Interaction : Signer un Formulaire

La logique principale est la suivante : un `Bureaucrat` peut-il signer un `Form` ?

1.  **La méthode `beSigned` dans `Form`** :
    Cette méthode est appelée sur une instance de `Form`. Elle prend un `Bureaucrat` en référence et vérifie si son grade est suffisant.
    ```cpp
    void Form::beSigned(const Bureaucrat& bureaucrat) {
        if (bureaucrat.getGrade() > this->gradeToSign) {
            // Le grade du bureaucrate est trop bas !
            throw Form::GradeTooLowException();
        }
        this->signed_ = true; // Le grade est suffisant, on signe.
    }
    ```
    Si le grade du `Bureaucrat` est numériquement supérieur (donc hiérarchiquement inférieur) au `gradeToSign` requis, une exception est lancée.

2.  **La méthode `signForm` dans `Bureaucrat`** :
    Cette méthode est plus "intelligente". Le `Bureaucrat` essaie de signer le formulaire et gère le résultat, qu'il soit positif ou négatif.
    ```cpp
    void Bureaucrat::signForm(Form& form) {
        try {
            form.beSigned(*this); // On essaie de signer le formulaire.
            std::cout << this->getName() << " a signé " << form.getName() << std::endl;
        } catch (const std::exception& e) {
            // Si beSigned a lancé une exception, on l'attrape ici.
            std::cout << this->getName() << " n'a pas pu signer " << form.getName() 
                      << " car " << e.what() << std::endl;
        }
    }
    ```
    Ici, le `try-catch` permet au programme de continuer même si la signature échoue. Au lieu de planter, il affiche un message d'erreur informatif.

### Différences Clés entre `ex00` et `ex01`

| Concept | ex00 | ex01 |
| :--- | :--- | :--- |
| **Objectif** | Gérer l'état interne d'une seule classe (`Bureaucrat`). | Gérer l'interaction entre deux classes (`Bureaucrat` et `Form`). |
| **Exceptions** | Utilisées pour valider les données à la création d'un objet. | Utilisées pour gérer les échecs d'une action entre deux objets (la signature). |
| **`try`/`catch`** | Principalement dans `main()` pour empêcher le programme de crasher. | Intégré dans une méthode de classe (`signForm`) pour gérer une interaction qui peut échouer sans arrêter le programme. |
| **Complexité** | Faible : une classe, des règles simples. | Moyenne : deux classes, des règles d'interaction, une gestion d'erreur plus fine. |

En passant de `ex00` à `ex01`, on apprend non seulement à créer des classes isolées, mais aussi à les faire collaborer de manière robuste, en anticipant et en gérant les erreurs qui peuvent survenir lors de leurs interactions. C'est un concept fondamental en programmation orientée objet.

---

## Exemples Concrets pour Mieux Comprendre `try-catch`

Voici quelques scénarios pour illustrer comment les blocs `try-catch` se comportent dans différentes situations.

### Scénario 1 : Tout se passe bien

Quand aucun `throw` n'est exécuté dans le bloc `try`, le bloc `catch` est simplement ignoré.

```cpp
#include "Bureaucrat.hpp"
#include <iostream>

int main() {
    try {
        // Création d'un bureaucrate avec un grade valide (75).
        Bureaucrat employeModele("John Doe", 75);
        std::cout << employeModele << std::endl; // Affiche: John Doe, bureaucrat grade 75

        // On essaie d'incrémenter son grade (de 75 à 74), ce qui est valide.
        employeModele.incrementGrade();
        std::cout << "Après promotion : " << employeModele << std::endl; // Affiche: Après promotion : John Doe, bureaucrat grade 74

    } catch (const std::exception& e) {
        // Ce code ne sera JAMAIS exécuté car aucune exception n'a été lancée.
        std::cout << "Une erreur est survenue : " << e.what() << std::endl;
    }

    std::cout << "Le programme continue normalement." << std::endl;
    return 0;
}
```
**Résultat attendu :**
```
John Doe, bureaucrat grade 75
Après promotion : John Doe, bureaucrat grade 74
Le programme continue normalement.
```

### Scénario 2 : Une exception est attrapée

Si un `throw` est exécuté, le bloc `try` s'arrête immédiatement et le contrôle est passé au bloc `catch`.

```cpp
#include "Bureaucrat.hpp"
#include <iostream>

int main() {
    try {
        // On crée un bureaucrate avec le grade le plus haut (1).
        Bureaucrat bigBoss("Le Patron", 1);
        std::cout << bigBoss << std::endl; // Affiche: Le Patron, bureaucrat grade 1

        // On tente de l'incrémenter. Son grade passerait à 0, ce qui est invalide.
        // La méthode incrementGrade() va lancer une GradeTooHighException.
        bigBoss.incrementGrade();

        // La ligne suivante ne sera JAMAIS atteinte.
        std::cout << "Cette promotion est impossible !" << std::endl;

    } catch (const std::exception& e) {
        // L'exception est attrapée ici.
        // e.what() retourne le message d'erreur défini dans la classe d'exception.
        std::cout << "Erreur critique : " << e.what() << std::endl; 
    }

    std::cout << "Le programme a géré l'erreur et peut continuer." << std::endl;
    return 0;
}
```
**Résultat attendu :**
```
Le Patron, bureaucrat grade 1
Erreur critique : Bureaucrat grade too high
Le programme a géré l'erreur et peut continuer.
```

### Scénario 3 : Interaction réussie (ex01)

Un bureaucrate avec un grade suffisant signe un formulaire.

```cpp
#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
    Bureaucrat maire("M. Le Maire", 10);
    Form permisDeConstruire("Permis de Construire", 20, 50); // Grade requis pour signer : 20

    std::cout << maire << std::endl;
    std::cout << permisDeConstruire << std::endl;

    // Le maire (grade 10) tente de signer un formulaire nécessitant un grade de 20.
    // 10 est meilleur que 20, donc la signature réussit.
    maire.signForm(permisDeConstruire);

    std::cout << permisDeConstruire << std::endl; // Le formulaire est maintenant signé.
    return 0;
}
```
**Résultat attendu :**
```
M. Le Maire, bureaucrat grade 10
Form Permis de Construire, signed false, grade to sign 20, grade to execute 50.
M. Le Maire a signé Permis de Construire
Form Permis de Construire, signed true, grade to sign 20, grade to execute 50.
```

### Scénario 4 : Interaction échouée (ex01)

Un bureaucrate tente de signer un formulaire mais son grade est trop bas.

```cpp
#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
    Bureaucrat stagiaire("Stagiaire", 120);
    Form autorisationSecrete("Autorisation Top Secrète", 5, 10); // Grade requis : 5

    std::cout << stagiaire << std::endl;
    std::cout << autorisationSecrete << std::endl;

    // Le stagiaire (grade 120) tente de signer un formulaire nécessitant un grade de 5.
    // La méthode signForm attrape l'exception et affiche un message d'erreur.
    stagiaire.signForm(autorisationSecrete);

    std::cout << autorisationSecrete << std::endl; // Le formulaire n'est toujours pas signé.
    return 0;
}
```
**Résultat attendu :**
```
Stagiaire, bureaucrat grade 120
Form Autorisation Top Secrète, signed false, grade to sign 5, grade to execute 10.
Stagiaire n'a pas pu signer Autorisation Top Secrète car Form grade too low
Form Autorisation Top Secrète, signed false, grade to sign 5, grade to execute 10.
```
Ces exemples montrent que `try-catch` est un outil puissant pour contrôler le flux de votre programme, gérer les erreurs de manière prévisible et créer des applications plus robustes et conviviales.

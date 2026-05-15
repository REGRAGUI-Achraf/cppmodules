# CPP05 ex03 - Guide debutant (Intern / Factory)

Ce README explique en detail l exercice ex03, surtout la classe Intern:
- pourquoi elle a ete ajoutee
- ce qu elle fait exactement
- comment elle fonctionne avec AForm et Bureaucrat
- des exemples concrets

---

## 1) Rappel rapide: ou on en etait avant ex03

Dans ex02, tu avais deja:
- Bureaucrat
- AForm (classe abstraite)
- 3 formulaires concrets:
  - ShrubberyCreationForm
  - RobotomyRequestForm
  - PresidentialPardonForm

Tu pouvais signer et executer des formulaires via le polymorphisme.

Mais la creation des formulaires etait encore "manuelle".
Tu devais savoir quelle classe concrete instancier.

---

## 2) Ce qui est nouveau dans ex03

La nouveaute principale est la classe Intern.

Intern est une fabrique de formulaires.
Son role n est pas de signer ni d executer.
Son role est seulement de creer le bon formulaire selon un nom texte.

Exemple d appel:

```cpp
Intern intern;
AForm* f = intern.makeForm("robotomy request", "Bender");
```

Tu lui donnes:
- le type demande (string)
- la cible (target)

Il te retourne:
- un pointeur AForm* vers le bon objet
- ou 0 si le nom n existe pas

---

## 3) Pourquoi on ajoute Intern

### A. Centraliser la creation
Sans Intern, la logique de creation est dispersee dans le code client.
Avec Intern, tout est dans un seul endroit: Intern.cpp.

### B. Reduire le couplage
Le code utilisateur depend moins des classes concretes.
Il parle en termes de AForm + nom texte.

### C. Faciliter l evolution
Si tu ajoutes un nouveau formulaire plus tard, tu modifies surtout Intern.
Le code appelant change peu.

### D. Montrer un pattern classique
Ex03 introduit le pattern Factory (Fabrique):
une classe specialisee cree les objets a ta place.

---

## 4) Lecture de ton code Intern

Fichiers:
- Intern.hpp
- Intern.cpp

La methode cle:

```cpp
AForm* makeForm(const std::string& name, const std::string& target) const;
```

Dans Intern.cpp, il y a:
1. Des fonctions de creation (une par type)
2. Une table qui mappe un nom texte a une fonction de creation
3. Une recherche sur la table pour trouver le bon createur

Flux interne:
1. Si name == "shrubbery creation" -> cree ShrubberyCreationForm(target)
2. Si name == "robotomy request" -> cree RobotomyRequestForm(target)
3. Si name == "presidential pardon" -> cree PresidentialPardonForm(target)
4. Sinon -> message d erreur + retour 0

### 4.1 Explication detaillee de cette fonction

Code concerne:

```cpp
AForm* Intern::makeForm(const std::string& name, const std::string& target) const {
    static const FormEntry forms[] = {
        {"shrubbery creation", &createShrubbery},
        {"robotomy request", &createRobotomy},
        {"presidential pardon", &createPresidential}
    };

    size_t i = 0;
    while (i < sizeof(forms) / sizeof(forms[0])) {
        if (name == forms[i].name) {
            AForm* form = forms[i].create(target);
            std::cout << "Intern creates " << name << std::endl;
            return form;
        }
        ++i;
    }

    std::cout << "Intern couldn't create form \"" << name << "\"" << std::endl;
    return 0;
}
```

Lecture simple ligne par ligne:

1. Signature de la methode
- `AForm* ...` veut dire que la methode retourne un pointeur vers la classe de base.
- Pourquoi: peu importe le type concret cree, on peut le manipuler via `AForm*` (polymorphisme).

2. Tableau `forms`
- Chaque entree contient:
  - un nom texte (ex: `robotomy request`)
  - une fonction de creation associee.
- Pourquoi: remplacer des blocs `if/else` repetitifs par une table claire et facile a etendre.

3. `size_t i = 0;`
- Initialise l index de parcours.
- Pourquoi `size_t`: type adapte aux tailles et index de tableaux.

4. Boucle `while`
- Condition: tant que `i` est dans les limites du tableau.
- `sizeof(forms) / sizeof(forms[0])` calcule le nombre d elements.
- Pourquoi: eviter de coder le nombre `3` en dur.

5. Test `if (name == forms[i].name)`
- Compare le nom demande avec le nom de l entree courante.
- Si ca matche: on a trouve le bon formulaire.

6. Creation dynamique
- `forms[i].create(target)` appelle la bonne fonction (`createRobotomy`, etc.).
- Retourne un objet cree avec `new`.
- Pourquoi dynamique: le type concret est decide a l execution.

7. Message de succes + `return form`
- Affiche ce qui a ete cree.
- Retour immediat du pointeur.

8. `++i`
- Passe a l entree suivante si pas de match.

9. Cas non trouve
- Si la boucle finit sans match: formulaire inconnu.
- Message d echec et `return 0`.

Important pour debutant:
- Si `makeForm` retourne 0, il faut tester avant d utiliser le pointeur.
- Si `makeForm` retourne un vrai pointeur, il faut faire `delete` apres usage.

Mini exemple d usage securise:

```cpp
Intern intern;
AForm* form = intern.makeForm("robotomy request", "Bender");

if (form) {
    Bureaucrat chief("Chief", 1);
    chief.signForm(*form);
    chief.executeForm(*form);
    delete form;
}
```

---

## 5) Intern n execute rien tout seul

Point tres important:
- Intern fabrique
- Bureaucrat signe
- Bureaucrat execute

Donc la responsabilite est proprement separee.

Exemple typique:

```cpp
Intern intern;
Bureaucrat chief("Chief", 1);

AForm* ppf = intern.makeForm("presidential pardon", "Arthur");
if (ppf) {
    chief.signForm(*ppf);
    chief.executeForm(*ppf);
    delete ppf;
}
```

---

## 6) Difference entre ex02 et ex03

### ex02
- Tu as le polymorphisme d execution
- Mais creation surtout manuelle

### ex03
- Tu gardes le polymorphisme d execution
- Tu ajoutes une abstraction de creation via Intern

En bref:
- ex02: comment utiliser des formulaires differents
- ex03: comment creer proprement ces formulaires

---

## 7) Exemples concrets (succès + erreur)

### Exemple A - Nom valide

```cpp
Intern intern;
AForm* form = intern.makeForm("robotomy request", "Bender");

if (form) {
    Bureaucrat boss("Boss", 1);
    boss.signForm(*form);
    boss.executeForm(*form);
    delete form;
}
```

Resultat attendu:
- Intern annonce la creation
- Signature possible si grade suffisant
- Execution possible si signe + grade suffisant

### Exemple B - Nom invalide

```cpp
Intern intern;
AForm* form = intern.makeForm("unknown form", "Nobody");

if (!form) {
    std::cout << "Formulaire inconnu, rien a executer" << std::endl;
}
```

Resultat attendu:
- Message d echec de creation
- form == 0
- le programme continue sans crash

---

## 8) Gestion memoire (important debutant)

Comme makeForm utilise new, tu dois faire delete.

Regle simple:
- si AForm* ptr est non nul et cree par makeForm, alors delete ptr quand tu as fini.

Sinon, fuite memoire.

---

## 9) Ce que tu apprends vraiment dans ex03

1. Separation des responsabilites
- Intern cree
- Bureaucrat agit
- AForm definit le contrat

2. Factory Pattern
- Creation centralisee et flexible

3. Code plus maintenable
- moins de duplication
- ajout de nouveaux formulaires plus simple

4. Robustesse
- nom invalide gere proprement
- le programme ne plante pas

---

## 10) Resume final en une phrase

Intern est ajoute dans ex03 pour professionnaliser la creation des formulaires: au lieu d instancier partout a la main, on delegue a une fabrique unique, ce qui rend le code plus propre, evolutif et pedagogiquement proche des bonnes pratiques C++ objet.

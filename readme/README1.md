# Guide Debutant C++ - Explication detaillee de ex02 et ex03

Ce document explique uniquement ex02 et ex03 de maniere simple, avec:
- ce qui est nouveau par rapport a ex01
- pourquoi on ajoute chaque element
- des exemples concrets

---

## ex02 - Passage a AForm, abstraction et polymorphisme

Dans ex01, on avait une classe concrete Form.
Dans ex02, on passe a une classe abstraite AForm et on cree des formulaires specialises.

### 1) Ce qui est nouveau dans ex02

1. Classe abstraite AForm
- Nouveau mot-cle: classe abstraite via methode virtuelle pure.
- Dans AForm, la methode execute est declaree ainsi:

```cpp
virtual void execute(Bureaucrat const& executor) const = 0;
```

- Le = 0 veut dire: AForm definit une regle mais pas le comportement final.
- Consequence: on ne peut pas faire AForm f; directement.

Pourquoi on ajoute ca:
- Parce que tous les formulaires partagent une base commune (nom, grade de signature, grade d execution, etat signe),
  mais leur action finale execute n est pas la meme.
- On evite de dupliquer la logique commune.

2. Formulaires concrets derives
- Nouveau: trois classes heritent de AForm:
  - ShrubberyCreationForm
  - RobotomyRequestForm
  - PresidentialPardonForm
- Chacune implemente sa propre version de execute.

Pourquoi on ajoute ca:
- Pour separer les responsabilites:
  - AForm gere les regles communes
  - chaque classe fille gere son action metier

3. Verification d execution avant action
- Avant d executer l action, il faut verifier:
  - le formulaire est signe
  - le bureaucrate a un grade suffisant pour executer
- Dans ton code, chaque formulaire fait ces verifications dans sa methode execute.

Pourquoi on ajoute ca:
- Pour garantir la securite metier:
  - impossible d executer un formulaire non signe
  - impossible d executer avec un grade insuffisant

4. Bureaucrat execute un AForm generique
- La methode Bureaucrat::executeForm prend un AForm const&.
- Elle appelle form.execute(*this).

Pourquoi c est puissant:
- Meme code pour tous les types de formulaires.
- C est le polymorphisme: une interface commune, comportements differents selon l objet reel.

---

### 2) Comment ex02 fonctionne dans le flux complet

1. Creation d un formulaire specialise
2. Signature via signForm (qui appelle beSigned)
3. Execution via executeForm (qui appelle execute du type reel)
4. Gestion d erreurs par exceptions

Exemple simplifie:

```cpp
Bureaucrat chief("Chief", 1);
RobotomyRequestForm robot("Bender");

chief.signForm(robot);      // passe si grade suffisant pour signer
chief.executeForm(robot);   // passe si signe + grade suffisant pour executer
```

Ce qui se passe vraiment:
- signForm appelle robot.beSigned(chief)
- executeForm appelle robot.execute(chief)
- robot.execute fait les checks puis l action specifique robotomy

---

### 3) Exemples concrets de chaque formulaire dans ex02

1. ShrubberyCreationForm
- Role: cree un fichier target_shrubbery contenant un arbre ASCII.

Exemple:
```cpp
ShrubberyCreationForm shrub("home");
manager.signForm(shrub);
chief.executeForm(shrub);
```

Resultat attendu:
- un fichier home_shrubbery est cree
- si le formulaire n est pas signe ou grade insuffisant: exception et message d erreur

2. RobotomyRequestForm
- Role: simule une robotomisation avec 50 pourcent de reussite.
- Le code utilise srand/time/rand pour le cote aleatoire.

Exemple:
```cpp
RobotomyRequestForm robot("Bender");
manager.signForm(robot);
chief.executeForm(robot);
```

Resultat attendu:
- affiche bruit de perceuse
- puis succes ou echec aleatoire

3. PresidentialPardonForm
- Role: annonce que la cible est pardonnee par Zaphod Beeblebrox.

Exemple:
```cpp
PresidentialPardonForm pardon("Arthur");
chief.signForm(pardon);
chief.executeForm(pardon);
```

Resultat attendu:
- message de pardon si toutes les conditions sont valides

---

### 4) Pourquoi ex02 est important pedagogiquement

Tu apprends 4 notions majeures de C++ objet:
1. Abstraction: AForm impose un contrat commun
2. Heritage: les formulaires specialises reutilisent la base
3. Polymorphisme: Bureaucrat manipule tous les formulaires via AForm
4. Robustesse: exceptions pour proteger les regles metier

---

## ex03 - Ajout de Intern et pattern Factory

Dans ex02, tu creais les formulaires directement avec new ClassName(...).
Dans ex03, on introduit Intern qui fabrique les formulaires a ta place.

### 1) Ce qui est nouveau dans ex03

1. Classe Intern
- Nouvelle methode principale:

```cpp
AForm* makeForm(const std::string& name, const std::string& target) const;
```

- Tu donnes un nom de formulaire (chaine) + target.
- Intern retourne un pointeur vers le bon type de formulaire.

Pourquoi on ajoute ca:
- Pour centraliser la creation des objets.
- Le code client n a plus besoin de connaitre chaque classe concrete.

2. Mapping nom -> fonction de creation
- Dans Intern.cpp, tu as un tableau d entrees:
  - nom texte
  - pointeur de fonction de creation

Idee:
- si name == "robotomy request" alors createRobotomy(target)
- si name == "shrubbery creation" alors createShrubbery(target)
- etc.

Pourquoi c est mieux qu une suite de if enorme:
- code lisible
- facile d ajouter un nouveau formulaire
- logique de construction rangee dans un seul endroit

3. Gestion du formulaire inconnu
- Si le nom n est pas reconnu, ton code affiche un message et retourne 0.

Pourquoi on ajoute ca:
- Pour eviter un crash
- Pour signaler proprement l erreur au code appelant

---

### 2) Pattern Factory explique simplement

Une Factory est une "usine" d objets.

Sans factory:
```cpp
AForm* f = new RobotomyRequestForm("Bender");
```

Avec factory:
```cpp
Intern i;
AForm* f = i.makeForm("robotomy request", "Bender");
```

Difference importante:
- Sans factory, le code appelant depend des classes concretes.
- Avec factory, il depend juste de AForm + un nom.

Pourquoi c est utile en vrai projet:
1. Moins de couplage
2. Code client plus propre
3. Ajout de nouveaux types plus simple

---

### 3) Exemple complet ex03 avec verification

```cpp
Intern intern;
Bureaucrat chief("Chief", 1);

AForm* a = intern.makeForm("robotomy request", "Bender");
AForm* b = intern.makeForm("unknown", "X");

if (a) {
    chief.signForm(*a);
    chief.executeForm(*a);
    delete a;
}

if (b) {
    chief.signForm(*b);
    chief.executeForm(*b);
    delete b;
}
```

Lecture de l exemple:
- a est cree correctement
- b vaut 0 si nom inconnu
- on teste if (ptr) avant utilisation
- on libere la memoire avec delete

---

### 4) Ce que tu dois retenir entre ex02 et ex03

1. ex02 t apprend a executer des comportements differents via une base abstraite commune
2. ex03 t apprend a creer ces objets de facon propre avec une usine (Intern)

En une phrase:
- ex02 = comment utiliser correctement les objets polymorphes
- ex03 = comment les creer proprement sans alourdir le code client

---

## Resume ultra simple

1. AForm existe pour partager les regles communes et forcer execute dans les classes filles.
2. Les formulaires specialises existent pour separer la logique metier de chaque action.
3. Les exceptions existent pour bloquer les actions invalides sans casser brutalement le programme.
4. Intern existe pour centraliser la creation et rendre le code evolutif.
5. Les pointeurs AForm* dans ex03 imposent une bonne discipline memoire: tester nullptr puis delete.

Si tu veux, je peux aussi ajouter une section Quiz debutant (10 questions) a la fin de ce fichier pour verifier que tu maitrises ex02/ex03 avant de passer au module suivant.

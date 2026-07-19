# Exercice 02 - Polymorphisme et `dynamic_cast`

## But de l'exercice

Le but de cet exercice est de comprendre comment identifier le vrai type d'un objet quand on ne connaît que son type de base.

Dans ce dossier, on a :

- une classe de base `Base`
- trois classes dérivées `A`, `B` et `C`
- une fonction `generate()` qui crée au hasard un objet de type `A`, `B` ou `C`
- deux fonctions `identify()` qui essaient de retrouver le type réel de l'objet

L'idée importante est la suivante :

- on manipule l'objet via un pointeur ou une référence de type `Base`
- mais l'objet réel en mémoire est en fait un `A`, un `B` ou un `C`
- il faut donc utiliser un cast dynamique pour découvrir le vrai type à l'exécution

## Structure du code

### `Base`

La classe `Base` contient un destructeur virtuel :

```cpp
class Base {
public:
    virtual ~Base() {}
};
```

Ce destructeur virtuel rend la classe polymorphique. C'est obligatoire pour que `dynamic_cast` fonctionne correctement.

### `A`, `B`, `C`

Les classes dérivées sont très simples :

```cpp
class A : public Base {};
class B : public Base {};
class C : public Base {};
```

Elles ne contiennent rien d'autre que l'héritage de `Base`.

### `generate()`

Cette fonction choisit aléatoirement une classe et crée un objet correspondant.

Exemple :

```cpp
Base* p = generate();
```

Selon le hasard, `p` peut pointer vers :

- un objet `A`
- un objet `B`
- un objet `C`

Le type visible est toujours `Base*`, mais le type réel derrière le pointeur est différent.

### `identify(Base*)`

Cette version reçoit un pointeur.

Elle teste successivement :

```cpp
if (dynamic_cast<A*>(p))
```

Puis `B`, puis `C`.

Si le cast réussit, cela veut dire que l'objet réel est du type demandé.

### `identify(Base&)`

Cette version reçoit une référence.

Ici, on ne peut pas tester `nullptr`, donc si le cast échoue, `dynamic_cast` lance une exception.

On utilise donc `try/catch` :

```cpp
try {
    dynamic_cast<A&>(p);
    std::cout << "A" << std::endl;
} catch (...) {
}
```

## Le rôle de `dynamic_cast`

`dynamic_cast` sert à faire une conversion sûre dans une hiérarchie de classes.

Contrairement à `static_cast`, il vérifie le type réel à l'exécution.

### Avec un pointeur

```cpp
Base* p = new B();

if (dynamic_cast<A*>(p))
    std::cout << "A" << std::endl;
else if (dynamic_cast<B*>(p))
    std::cout << "B" << std::endl;
else if (dynamic_cast<C*>(p))
    std::cout << "C" << std::endl;
```

Si `p` pointe vers un `B`, alors :

- `dynamic_cast<A*>(p)` retourne `nullptr`
- `dynamic_cast<B*>(p)` retourne un pointeur valide

Donc on détecte que l'objet est un `B`.

### Avec une référence

```cpp
Base& ref = *p;

try {
    dynamic_cast<A&>(ref);
    std::cout << "A" << std::endl;
} catch (...) {}
```

Si `ref` n'est pas un `A`, le cast lance une exception.

## Exemples concrets

### Exemple 1 : objet `A`

```cpp
Base* p = new A();
```

Résultat attendu :

- `identify(p)` affiche `A`
- `identify(*p)` affiche `A`

### Exemple 2 : objet `B`

```cpp
Base* p = new B();
```

Résultat attendu :

- `identify(p)` affiche `B`
- `identify(*p)` affiche `B`

### Exemple 3 : objet `C`

```cpp
Base* p = new C();
```

Résultat attendu :

- `identify(p)` affiche `C`
- `identify(*p)` affiche `C`

## Ce que montre le `main`

Dans `main.cpp`, le programme :

1. appelle `srand(...)` pour initialiser le hasard
2. boucle 10 fois
3. crée un objet aléatoire avec `generate()`
4. affiche son type avec `identify(Base*)`
5. affiche encore son type avec `identify(Base&)`
6. détruit l'objet avec `delete`

Le programme montre donc que même si on manipule tout via `Base`, on peut retrouver le vrai type au moment de l'exécution.

## À retenir

- `Base` doit être polymorphique pour que `dynamic_cast` fonctionne
- `dynamic_cast` sert à vérifier le type réel d'un objet
- avec un pointeur, un cast raté retourne `nullptr`
- avec une référence, un cast raté lance une exception
- cet exercice t'apprend à reconnaître le type concret derrière une interface de base

## Résumé très simple

On fabrique un objet au hasard parmi `A`, `B` et `C`.
On le traite comme un `Base*`.
Puis on utilise `dynamic_cast` pour découvrir son vrai type.

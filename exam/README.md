# bigint

Ce sujet demande d’implémenter un entier non signé à précision arbitraire, c’est-à-dire un type capable de stocker des nombres beaucoup plus grands que `SIZE_MAX`, sans perte de précision.

Le principe est simple : au lieu d’utiliser un type binaire classique (`int`, `unsigned long long`, etc.), on stocke le nombre sous une forme adaptée aux opérations demandées. Ici, l’idée retenue est une représentation en base 10, sous forme de chaîne de caractères.

## Objectif du sujet

Le but est de créer une classe `bigint` dans [bigint.hpp](bigint.hpp) et [bigint.cpp](bigint.cpp) qui sache :

- stocker un entier positif de taille arbitraire ;
- additionner deux `bigint` ;
- comparer deux `bigint` ;
- faire des décalages de chiffres en base 10 avec `<<` et `>>` ;
- s’afficher avec `std::ostream` via `operator<<`.

Le fichier [main.cpp](main.cpp) sert de test attendu : il doit compiler et fonctionner avec la classe.

## Idée de représentation

Dans cette implémentation, un `bigint` est représenté par un membre `std::string value`.

Exemples :

- `0` est stocké comme `"0"`
- `42` est stocké comme `"42"`
- `1337` est stocké comme `"1337"`

Cette représentation est pratique pour :

- additionner chiffre par chiffre, en partant de la droite ;
- comparer d’abord la longueur, puis le contenu ;
- simuler un décalage décimal en ajoutant ou en retirant des zéros.

## Constructeurs

La classe fournit plusieurs façons de créer un `bigint` :

- constructeur par défaut : crée `0` ;
- constructeur à partir d’un `unsigned long long` ;
- constructeur à partir d’une chaîne de caractères ;
- constructeur de copie.

Exemples :

- `bigint a;` donne `0` ;
- `bigint b(42);` donne `42` ;
- `bigint c("001337");` devient `1337` après suppression des zéros initiaux.

## Normalisation de la valeur

Un point important du sujet est l’absence de zéros en tête dans l’affichage.

La méthode `remove_zeros()` sert à normaliser la chaîne interne :

- `"00042"` devient `"42"` ;
- `"0000"` devient `"0"`.

Cela garantit que l’affichage reste propre et que les comparaisons fonctionnent correctement.

## Addition

L’addition se fait comme à la main, de droite à gauche :

1. on prend le dernier chiffre de chaque nombre ;
2. on additionne avec la retenue éventuelle ;
3. on écrit le chiffre de résultat ;
4. on propage la retenue ;
5. on continue jusqu’à avoir traité tous les chiffres.

Exemple :

```text
  1337
+   42
------
  1379
```

Le code de [bigint.cpp](bigint.cpp) utilise une fonction auxiliaire `addStrings()` pour faire cette addition de chaînes.

Opérateurs associés :

- `operator+` pour créer un nouveau résultat ;
- `operator+=` pour modifier l’objet courant ;
- `operator++` préfixé et postfixé.

## Comparaison

Pour comparer deux grands entiers, il faut d’abord comparer leur taille décimale :

- un nombre à 5 chiffres est plus grand qu’un nombre à 3 chiffres ;
- si les longueurs sont égales, on compare les chaînes lexicographiquement.

Exemples :

- `"99" > "100"` est faux car `99` a moins de chiffres que `100` ;
- `"1337" > "0420"` devient `1337 > 420` après normalisation.

La classe implémente :

- `==`
- `!=`
- `<`
- `>`
- `<=`
- `>=`

## Décalage de chiffres

Le sujet demande un “digitshift”, c’est-à-dire un décalage en base 10, pas un décalage binaire.

### Décalage à gauche `<<`

Décaler à gauche de `n` chiffres revient à multiplier par `10^n`.

Exemples :

- `42 << 3 = 42000`
- `1337 << 2 = 133700`

Dans l’implémentation, cela se fait en ajoutant `n` zéros à la fin de la chaîne.

### Décalage à droite `>>`

Décaler à droite de `n` chiffres revient à enlever les `n` derniers chiffres.

Exemples :

- `1337 >> 2 = 13`
- `42000 >> 3 = 42`

Si on décale trop loin, le résultat devient `0`.

### Versions avec affectation

Les opérateurs suivants existent aussi :

- `<<=`
- `>>=`

Ils modifient directement l’objet courant.

## Décalage avec un `bigint`

La classe propose aussi des versions de `<<`, `>>`, `<<=`, `>>=` qui prennent un autre `bigint` comme argument.

Dans l’implémentation actuelle, la valeur du décalage est convertie en `unsigned long long` via `stringToULL()`.

Cela signifie que le décalage doit rester dans une plage compatible avec ce type.

## Affichage

L’opérateur d’affichage `operator<<` permet d’écrire un `bigint` directement dans un flux :

```cpp
bigint a(42);
std::cout << a << std::endl;
```

Le résultat doit être en base 10, sans zéro inutile en tête.

## Ce que le `main` attend

Le fichier [main.cpp](main.cpp) teste plusieurs comportements :

- construction par défaut et par valeur ;
- copie ;
- affichage ;
- addition ;
- incrémentation préfixée et postfixée ;
- décalages gauche et droite ;
- comparaisons.

Autrement dit, si votre classe respecte l’interface de [bigint.hpp](bigint.hpp), le `main` doit s’exécuter sans erreur et produire les résultats attendus.

## Résumé rapide

Ce sujet consiste à simuler un entier non signé “illimité” à l’aide d’une chaîne de caractères. L’essentiel est de bien gérer :

- la représentation interne ;
- la suppression des zéros en tête ;
- l’addition avec retenue ;
- les comparaisons par longueur puis par valeur ;
- les décalages décimaux, pas binaires.

Si vous voulez, je peux aussi ajouter une section plus pédagogique avec un exemple pas à pas de l’addition et du décalage.
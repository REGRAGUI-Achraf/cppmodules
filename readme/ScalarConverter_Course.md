# 🎓 Cours Complet — ScalarConverter (C++ Module 06 — 42 School)

---

## 1. OBJECTIF GLOBAL

### Pourquoi cet exercice existe

Le Module 06 de 42 porte sur **les casts en C++**. L'exercice ScalarConverter est le premier,
et son but pédagogique est triple :

1. **Comprendre les types scalaires** et leurs limites intrinsèques
2. **Maîtriser les conversions explicites** avec `static_cast<>`
3. **Apprendre à parser une chaîne de caractères** pour détecter un type à la volée

En C, on pouvait écrire `(int)42.5` et ça compilait silencieusement même si c'était un désastre.
C++ introduit des casts nommés et explicites pour rendre **chaque conversion visible et intentionnelle**.

### Ce qu'on apprend profondément

- La différence entre conversion **implicite** et **explicite**
- Les **narrowing conversions** (conversions réductrices)
- Les **overflow** et les **underflow**
- La **représentation en mémoire** des types numériques
- Le **parsing** de chaînes de caractères
- La conception d'une **classe utilitaire non-instanciable**

---

## 2. TYPES SCALAIRES EN PROFONDEUR

Un type scalaire est un type qui contient **une seule valeur** (par opposition à un tableau ou une struct).

---

### 2.1 `char` — 1 octet

```
Mémoire : [  8 bits  ]
Plage    : -128 à 127 (signed) ou 0 à 255 (unsigned)
Taille   : 1 octet = 8 bits

Bit 7    Bit 6    Bit 5  ...  Bit 0
  |        |        |           |
signe    valeur   valeur  ... valeur
```

Le `char` est **le seul type scalaire qui représente un caractère**.
Chaque valeur correspond à un code ASCII.

```
Valeur numérique | Caractère ASCII
       65        |      'A'
       97        |      'a'
       48        |      '0'
       42        |      '*'
       10        |      '\n'  (non-displayable)
        7        |      '\a'  (non-displayable)
```

**Displayable** : caractères de 32 (' ') à 126 ('~').
En dehors → "Non displayable".

Exemple :
```cpp
char c = 'A';   // stocke 65 en mémoire
std::cout << c;          // affiche 'A'
std::cout << (int)c;     // affiche 65
```

---

### 2.2 `int` — 4 octets

```
Mémoire : [  8 bits  |  8 bits  |  8 bits  |  8 bits  ]
Plage   : -2 147 483 648  à  +2 147 483 647
Taille  : 4 octets = 32 bits

 Bit 31  (signe)   Bits 30→0 (valeur)
    |                   |
    1 = négatif         valeur absolue en complément à 2
```

Le `int` est le type entier "naturel" pour le processeur.
Toute valeur trop grande → **overflow** (comportement indéfini en C++98).

```
INT_MAX  =  2147483647
INT_MIN  = -2147483648
```

---

### 2.3 `float` — 4 octets (IEEE 754 simple précision)

```
Mémoire : [  1 bit  |  8 bits  |  23 bits  ]
           signe     exposant   mantisse

Exemple : 42.0f
  Binaire : 0 10000100 01010000000000000000000
  Signe   : 0        (positif)
  Exposant: 10000100 = 132 → 132 - 127 = 5
  Mantisse: 1.0101... → 1.3125 × 2^5 = 42.0
```

**Précision** : environ **7 chiffres significatifs**.
```
float f = 1234567.89f;
// Stocké comme : 1234568.0f  ← perte de précision !
```

Valeurs spéciales encodées :
```
+infini  : exposant = 11111111, mantisse = 0,  signe = 0
-infini  : exposant = 11111111, mantisse = 0,  signe = 1
NaN      : exposant = 11111111, mantisse ≠ 0
```

---

### 2.4 `double` — 8 octets (IEEE 754 double précision)

```
Mémoire : [  1 bit  |  11 bits  |  52 bits  ]
           signe      exposant    mantisse

Précision : ~15-16 chiffres significatifs
Plage     : ±1.7 × 10^308
```

**Comparaison des précisions :**

```
Type    | Taille | Chiffres sig. | Plage (approx.)
--------|--------|---------------|------------------
char    | 1 o    | N/A           | -128 à 127
int     | 4 o    | N/A (exact)   | ±2.1 milliards
float   | 4 o    | ~7            | ±3.4 × 10^38
double  | 8 o    | ~15-16        | ±1.7 × 10^308
```

---

### 2.5 Schéma comparatif ASCII

```
char   : [########]
          8 bits

int    : [########|########|########|########]
          32 bits

float  : [s|eeeeeeee|mmmmmmmmmmmmmmmmmmmmmmm]
          1+8+23 = 32 bits

double : [s|eeeeeeeeeee|mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm]
          1+11+52 = 64 bits
```

---

## 3. PARSING — Transformer une string en valeur

### 3.1 Qu'est-ce que le parsing ?

**Parser** = analyser une chaîne de caractères pour en extraire le sens.

```
Input : "42.0f"      → c'est un float
Input : "42"         → c'est un int
Input : "'a'"        → c'est un char
Input : "3.14"       → c'est un double
Input : "nanf"       → c'est un pseudo-literal float
```

La chaîne n'est que du texte. Notre job : **deviner le type**, **extraire la valeur**.

---

### 3.2 Algorithme de détection — Étape par étape

```
INPUT STRING
     │
     ▼
┌─────────────────────────────────────────────────────┐
│ ÉTAPE 1 : Est-ce un pseudo-literal ?                │
│  "nan", "nanf", "+inf", "-inf", "+inff", "-inff"    │
└────────────────────┬────────────────────────────────┘
                     │ Non
                     ▼
┌─────────────────────────────────────────────────────┐
│ ÉTAPE 2 : Est-ce un char ?                          │
│  Longueur == 3 ET commence par ' ET finit par '     │
│  Exemple : 'a', 'Z', '*'                            │
└────────────────────┬────────────────────────────────┘
                     │ Non
                     ▼
┌─────────────────────────────────────────────────────┐
│ ÉTAPE 3 : Est-ce un float ?                         │
│  Contient un '.' ET finit par 'f' ou 'F'            │
│  Exemple : 4.2f, -0.5f, 0.0f                        │
└────────────────────┬────────────────────────────────┘
                     │ Non
                     ▼
┌─────────────────────────────────────────────────────┐
│ ÉTAPE 4 : Est-ce un double ?                        │
│  Contient un '.' (sans 'f' à la fin)                │
│  Exemple : 4.2, -3.14, 0.0                          │
└────────────────────┬────────────────────────────────┘
                     │ Non
                     ▼
┌─────────────────────────────────────────────────────┐
│ ÉTAPE 5 : C'est un int                              │
│  Que des chiffres (avec éventuellement un '-')      │
│  Exemple : 42, -100, 0                              │
└─────────────────────────────────────────────────────┘
```

---

### 3.3 Détail de chaque détection

#### Pseudo-literal
```cpp
bool isPseudoLiteral(const std::string& s) {
    return (s == "nan" || s == "nanf" ||
            s == "+inf" || s == "-inf" ||
            s == "+inff" || s == "-inff");
}
```

#### Char
```cpp
bool isChar(const std::string& s) {
    return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
    // 'a' → length=3, s[0]='\'', s[1]='a', s[2]='\''
}
```

#### Float
```cpp
bool isFloat(const std::string& s) {
    // Doit se terminer par 'f' et contenir un '.'
    // Exemples valides : 4.2f, -0.0f, .5f
    if (s[s.length()-1] != 'f') return false;
    std::string withoutF = s.substr(0, s.length()-1);
    // withoutF doit être un nombre décimal valide
    return hasDecimalPoint(withoutF) && isValidDecimal(withoutF);
}
```

#### Double
```cpp
bool isDouble(const std::string& s) {
    // Contient un '.' mais ne finit pas par 'f'
    return hasDecimalPoint(s) && isValidDecimal(s);
}
```

#### Int
```cpp
bool isInt(const std::string& s) {
    // Optionnel '-' au début, puis que des chiffres
    size_t start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    for (size_t i = start; i < s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}
```

---

### 3.4 Conversion string → valeur numérique

Fonctions autorisées par le sujet :

```cpp
#include <cstdlib>    // atoi, atof, strtol, strtod

// String → int
int i = std::atoi("42");          // → 42
int i = std::strtol("42", NULL, 10); // plus robuste

// String → float
float f = static_cast<float>(std::atof("4.2f"));  // atof retourne double

// String → double
double d = std::atof("3.14");     // → 3.14

// String → char
char c = str[1];  // pour "'a'" → str[1] = 'a'
```

---

## 4. PSEUDO-LITERALS — Les valeurs spéciales

### 4.1 Pourquoi existent-ils ?

La norme IEEE 754 (qui définit float et double) a prévu des valeurs
**hors de la plage normale** pour représenter des cas mathématiques particuliers :

```
Cas mathématique        | Valeur IEEE 754
------------------------|------------------
1.0 / 0.0              | +infini (+inf)
-1.0 / 0.0             | -infini (-inf)
0.0 / 0.0              | NaN (Not a Number)
sqrt(-1.0)             | NaN
log(-1.0)              | NaN
```

Ces valeurs **ne peuvent exister que dans float et double**, car `int` et `char`
n'ont pas de représentation binaire pour l'infini ou NaN.

---

### 4.2 Représentation mémoire des pseudo-literals

```
+inf (float, 32 bits) :
  [0 | 11111111 | 00000000000000000000000]
   signe  exposant    mantisse = 0

-inf (float, 32 bits) :
  [1 | 11111111 | 00000000000000000000000]
   signe  exposant    mantisse = 0

NaN (float, 32 bits) :
  [0 | 11111111 | 10000000000000000000000]
   signe  exposant    mantisse ≠ 0
```

La règle IEEE 754 : si l'exposant est **tous des 1** :
- mantisse == 0 → infini
- mantisse != 0 → NaN

---

### 4.3 Notations dans le sujet

| String d'entrée | Type détecté | Valeur               |
|-----------------|--------------|----------------------|
| `"nan"`         | double       | NaN double           |
| `"nanf"`        | float        | NaN float            |
| `"+inf"`        | double       | +infini double       |
| `"-inf"`        | double       | -infini double       |
| `"+inff"`       | float        | +infini float        |
| `"-inff"`       | float        | -infini float        |

---

### 4.4 Pourquoi les conversions sont "impossible" ?

Pour `nan` et `inf`, la conversion vers `char` ou `int` est **undefined behavior**.
Ces types n'ont pas de représentation pour ces valeurs.

```
nan  → char : IMPOSSIBLE (char n'a pas de NaN)
nan  → int  : IMPOSSIBLE (int n'a pas de NaN)
nan  → float: nanf        (float peut stocker NaN)
nan  → double: nan        (double peut stocker NaN)

+inf → char : IMPOSSIBLE
+inf → int  : IMPOSSIBLE (pas d'infini pour int)
+inf → float: +inff
+inf → double: +inf
```

En C++, convertir NaN ou inf vers int produit un **comportement indéfini**.
Il faut détecter ces cas AVANT de faire le cast.

```cpp
// Vérification avec <cmath>
#include <cmath>
if (std::isinf(d) || std::isnan(d)) {
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible"  << std::endl;
}
```

---

## 5. STATIC_CAST — La conversion explicite

### 5.1 Pourquoi static_cast existe

En C, on pouvait écrire :
```c
double d = 42.9;
int i = (int)d;  // Cast C-style : brutal, silencieux
```

C++ introduit `static_cast<>` pour rendre les conversions **visibles** et
**grep-pables** dans le code.

```
C-style cast     :  (int)d         → invisible, ambigu
static_cast<>    :  static_cast<int>(d)  → explicite, cherchable
```

---

### 5.2 Fonctionnement interne

```
static_cast<ToType>(fromValue)
         │                 │
         │                 └── Valeur à convertir
         └── Type cible

Le compilateur vérifie à la compilation que la conversion est connue.
Il insère les instructions de conversion nécessaires.
```

---

### 5.3 Conversions sans perte (safe casts)

```
char → int    : toujours safe (int est plus grand)
int  → double : presque toujours safe (double a 15 sig. digits)
float→ double : toujours safe (double est plus précis)

Schéma :
char ──→ int ──→ long ──→ double
  └──────────────────→ float
```

---

### 5.4 Narrowing conversions (conversions réductrices)

```
double → int  : PERTE (partie décimale tronquée)
double → float: PERTE (précision réduite)
int    → char : PERTE (si > 127 ou < -128)

Exemple :
double d = 4.9;
int i = static_cast<int>(d);  // i = 4 (tronqué, pas arrondi !)

double d2 = 12345678901.0;
int i2 = static_cast<int>(d2);  // OVERFLOW → comportement indéfini !
```

---

### 5.5 Visualisation des conversions possibles

```
         ┌──────────────────────────────────────────────────┐
         │                                                  │
  STRING │  PARSING  →  TYPE DÉTECTÉ  →  CONVERSIONS       │
         │                                                  │
         │  "42.0f"  ────→  float      ─→ char  : '*'      │
         │                              ─→ int   : 42       │
         │                              ─→ float : 42.0f    │
         │                              ─→ double: 42.0     │
         │                                                  │
         │  "nan"    ────→  double     ─→ char  : impossible│
         │                              ─→ int   : impossible│
         │                              ─→ float : nanf     │
         │                              ─→ double: nan      │
         └──────────────────────────────────────────────────┘
```

---

### 5.6 Overflow — Quand le conteneur est trop petit

```
Valeur : 300
  └──→ Conversion vers char (max 127)

  300 en binaire : 100101100
                   ^^^^^^^^^
                   9 bits → ne rentre pas dans 8 bits !

  Résultat : comportement indéfini / valeur tronquée

  300 % 256 = 44 → mais c'est UNDEFINED en C++ signé !
```

Pour vérifier l'overflow avant de caster :
```cpp
if (d >= 0 && d <= 127 && !std::isnan(d) && !std::isinf(d)) {
    // Safe to cast to char
}
if (d >= INT_MIN && d <= INT_MAX && !std::isnan(d) && !std::isinf(d)) {
    // Safe to cast to int
}
```

---

## 6. ARCHITECTURE DU PROJET

### 6.1 Pourquoi non-instanciable ?

La classe `ScalarConverter` ne contient **aucun état**. Elle n'a besoin de
stocker aucune donnée membre. Sa seule raison d'exister est d'offrir
la fonction `convert()`.

Rendre une classe non-instanciable = la transformer en **namespace avec des méthodes**.

**Technique** : constructeur privé.

```cpp
class ScalarConverter {
private:
    ScalarConverter();                              // Pas de construction
    ScalarConverter(const ScalarConverter& other);  // Pas de copie
    ScalarConverter& operator=(const ScalarConverter& other); // Pas d'affectation

public:
    static void convert(const std::string& literal); // La seule méthode
};
```

Si quelqu'un essaie :
```cpp
ScalarConverter sc;  // ERREUR DE COMPILATION : constructeur privé
```

---

### 6.2 Pourquoi static ?

Une méthode `static` appartient à la **classe**, pas à une instance.

```cpp
// Avec méthode non-static (nécessite une instance) :
ScalarConverter sc;
sc.convert("42");   // Besoin de créer l'objet

// Avec méthode static (appel direct) :
ScalarConverter::convert("42");  // Pas d'instance nécessaire
```

---

### 6.3 Structure des fichiers

```
ex00/
├── Makefile
├── main.cpp
├── ScalarConverter.hpp
└── ScalarConverter.cpp
```

#### ScalarConverter.hpp
```
┌─────────────────────────────────────────┐
│  #ifndef SCALARCONVERTER_HPP            │
│  #define SCALARCONVERTER_HPP            │
│                                         │
│  class ScalarConverter {                │
│  private:                               │
│      ScalarConverter();                 │
│      ScalarConverter(const SC&);        │
│      SC& operator=(const SC&);          │
│                                         │
│  public:                                │
│      static void convert(const str&);   │
│  };                                     │
│                                         │
│  #endif                                 │
└─────────────────────────────────────────┘
```

#### ScalarConverter.cpp
```
┌─────────────────────────────────────────┐
│  Fonctions de détection (isChar, etc.)  │
│  Fonctions d'affichage (printChar, etc.)│
│  ScalarConverter::convert() principale  │
└─────────────────────────────────────────┘
```

#### Makefile
```makefile
NAME    = convert
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98
SRCS    = main.cpp ScalarConverter.cpp
OBJS    = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

---

## 7. ROADMAP DE DÉVELOPPEMENT

```
ÉTAPE 1 : Structure de base
─────────────────────────────
✓ Créer ScalarConverter.hpp avec la classe non-instanciable
✓ Créer ScalarConverter.cpp vide avec les includes
✓ Créer main.cpp qui appelle ScalarConverter::convert(argv[1])
✓ Créer Makefile
→ Compiler et vérifier que ça compile

ÉTAPE 2 : Fonctions de détection
─────────────────────────────────
✓ isPseudoLiteral(str)
✓ isChar(str)
✓ isFloat(str)
✓ isDouble(str)
✓ isInt(str)
→ Tester chaque fonction indépendamment avec des prints

ÉTAPE 3 : Gestion des pseudo-literals
──────────────────────────────────────
✓ Afficher "impossible" pour char et int
✓ Afficher la valeur correcte pour float et double
✓ Utiliser std::numeric_limits<float>::infinity() ou strtof/strtod
→ Tester : nan, nanf, +inf, -inf, +inff, -inff

ÉTAPE 4 : Conversion depuis char
─────────────────────────────────
✓ Extraire le char de la string ('a' → 'a')
✓ Afficher le char (avec vérification displayable)
✓ Caster vers int, float, double
→ Tester : 'a', 'Z', ' ', '*'

ÉTAPE 5 : Conversion depuis int
────────────────────────────────
✓ Parser l'int avec strtol (pour détecter overflow)
✓ Vérifier si convertible en char
✓ Afficher les 4 valeurs
→ Tester : 0, 42, -42, 2147483647, -2147483648

ÉTAPE 6 : Conversion depuis float
──────────────────────────────────
✓ Parser le float avec strtof
✓ Vérifier isinf/isnan
✓ Afficher les 4 valeurs avec bonne précision (.0f si entier)
→ Tester : 0.0f, 42.0f, 4.2f, -4.2f

ÉTAPE 7 : Conversion depuis double
────────────────────────────────────
✓ Parser le double avec strtod
✓ Vérifier isinf/isnan
✓ Afficher les 4 valeurs
→ Tester : 0.0, 42.0, 4.2, -4.2

ÉTAPE 8 : Affichage de la précision
─────────────────────────────────────
✓ Pour float et double : si la valeur est entière, afficher ".0"
   Exemple : 42.0f et 42.0, pas 42f et 42
✓ Utiliser std::fixed et std::setprecision si besoin

ÉTAPE 9 : Tests complets
─────────────────────────
✓ Tous les exemples du sujet
✓ Edge cases (INT_MAX, INT_MIN, très grands doubles, etc.)
✓ Entrées invalides
```

---

## 8. DIAGRAMMES ASCII

### 8.1 Flux global du programme

```
argv[1]
   │
   ▼
ScalarConverter::convert(literal)
   │
   ├──→ detectType(literal)
   │         │
   │         ├──→ PSEUDO_LITERAL
   │         ├──→ CHAR
   │         ├──→ INT
   │         ├──→ FLOAT
   │         └──→ DOUBLE
   │
   ├──→ parseValue(literal, type)
   │         │
   │         └──→ valeur native (double interne)
   │
   └──→ printAll(value, type)
             │
             ├──→ printChar(value)
             ├──→ printInt(value)
             ├──→ printFloat(value)
             └──→ printDouble(value)
```

---

### 8.2 Arbre de décision du parsing

```
          ┌─────────────────────┐
          │   str == "nan"      │──→ YES → PSEUDO (nan)
          │   str == "nanf" etc │
          └─────────┬───────────┘
                    │ NO
                    ▼
          ┌─────────────────────┐
          │  len==3 && '[0]==\' │──→ YES → CHAR
          │  && s[2]==\'        │
          └─────────┬───────────┘
                    │ NO
                    ▼
          ┌─────────────────────┐
          │  ends with 'f'      │──→ YES → FLOAT
          │  AND has '.'        │
          └─────────┬───────────┘
                    │ NO
                    ▼
          ┌─────────────────────┐
          │  has '.'            │──→ YES → DOUBLE
          └─────────┬───────────┘
                    │ NO
                    ▼
                  INT
```

---

### 8.3 Affichage char — décision

```
value (double interne)
    │
    ├──→ isnan(value) OR isinf(value)  ──→ "char: impossible"
    │
    ├──→ value < 0 OR value > 127      ──→ "char: impossible"
    │
    ├──→ (char)value non-affichable    ──→ "char: Non displayable"
    │    (value < 32 OR value > 126)
    │
    └──→ Sinon                         ──→ "char: 'X'"
```

---

### 8.4 Overflow vers int

```
INT_MIN = -2147483648
INT_MAX =  2147483647

      ◄────────────── domaine int ──────────────►
──────|─────────────────────────────────────────|──────
      │                                         │
   INT_MIN                                   INT_MAX

Si value < INT_MIN ou value > INT_MAX :
     → "int: impossible"

Si isnan(value) ou isinf(value) :
     → "int: impossible"
```

---

### 8.5 Précision float/double — formatage

```
Valeur : 42.0

float  : std::cout << std::fixed << std::setprecision(1) << 42.0f << "f"
         → "42.0f"

double : std::cout << std::fixed << std::setprecision(1) << 42.0
         → "42.0"

Valeur : 4.2

float  : std::cout << 4.2f  → "4.2f"
double : std::cout << 4.2   → "4.2"
```

**Règle :** si la valeur cast en `long` puis recast en `double` est identique,
c'est un entier → afficher avec `.0`.

```cpp
bool isWholeNumber = (value == static_cast<double>(static_cast<long>(value)));
```

---

## 9. ERREURS CLASSIQUES DES ÉTUDIANTS DE 42

### Erreur 1 : Oublier l'ordre de détection

```cpp
// MAUVAIS : isInt() avant isFloat()
// "42.0f" passerait d'abord dans isInt() → incorrect !

// BON : toujours tester pseudo-literal → char → float → double → int
```

### Erreur 2 : Caster directement sans vérifier l'overflow

```cpp
// MAUVAIS :
double d = 1e300;
int i = static_cast<int>(d);  // UNDEFINED BEHAVIOR

// BON :
if (d > INT_MAX || d < INT_MIN) {
    std::cout << "int: impossible" << std::endl;
} else {
    std::cout << "int: " << static_cast<int>(d) << std::endl;
}
```

### Erreur 3 : Ne pas tester `isnan` avant de comparer

```cpp
double nan = std::numeric_limits<double>::quiet_NaN();

// MAUVAIS (NaN ne se compare pas à lui-même !) :
if (nan > INT_MAX || nan < INT_MIN) // Toujours FAUX pour NaN !

// BON :
if (std::isnan(nan) || std::isinf(nan)) { ... }
```

### Erreur 4 : Affichage de float sans le 'f'

```
Attendu : "float: 42.0f"
Obtenu  : "float: 42.0"   ← oublié le 'f' !
```

```cpp
std::cout << "float: " << f << "f" << std::endl;
// Et si la valeur est nan/inf :
std::cout << "float: nanf" << std::endl;
std::cout << "float: +inff" << std::endl;
```

### Erreur 5 : Ne pas gérer "0" comme affichage décimal

```
Pour 0.0 :
Attendu : "float: 0.0f"
Obtenu  : "float: 0f"   ← mauvais !
```

Toujours utiliser `std::fixed << std::setprecision(1)` pour les entiers.

### Erreur 6 : Ne pas inclure les bons headers

```cpp
#include <iostream>   // cout
#include <string>     // string
#include <cmath>      // isnan, isinf
#include <climits>    // INT_MAX, INT_MIN
#include <cfloat>     // FLT_MAX, DBL_MAX
#include <cstdlib>    // atoi, atof, strtod
#include <iomanip>    // setprecision
```

### Erreur 7 : `atof` pour les pseudo-literals

`atof("nan")` retourne NaN, `atof("+inf")` peut retourner inf.
Mais c'est **non-standard** ! Utiliser `strtod` ou `strtof` qui gèrent mieux ces cas,
ou les détecter manuellement et utiliser `std::numeric_limits`.

---

## 10. PSEUDO-CODE COMPLET

```
PROGRAMME convert :
INPUT : argv[1] (une string)

FONCTION convert(literal):

    1. type = detectType(literal)

    2. SI type == PSEUDO_LITERAL:
          SI "nan" ou "nanf":
              d = NaN (via strtod ou numeric_limits)
          SINON SI "+inf" ou "+inff":
              d = +infini
          SINON SI "-inf" ou "-inff":
              d = -infini
          ALLER EN 5

    3. SI type == CHAR:
          c = literal[1]
          d = (double)c
          ALLER EN 5

    4. SI type == INT:
          i = strtol(literal)
          VÉRIFIER overflow
          d = (double)i
          ALLER EN 5

    5. SI type == FLOAT:
          f = strtof(literal)
          d = (double)f
          ALLER EN 5

    6. SI type == DOUBLE:
          d = strtod(literal)

    5. AFFICHER char(d):
          SI isnan(d) OU isinf(d) OU d<0 OU d>127:
              afficher "impossible"
          SINON SI d < 32 OU d > 126:
              afficher "Non displayable"
          SINON:
              afficher "'" + (char)d + "'"

    6. AFFICHER int(d):
          SI isnan(d) OU isinf(d) OU d < INT_MIN OU d > INT_MAX:
              afficher "impossible"
          SINON:
              afficher (int)d

    7. AFFICHER float(d):
          SI isnan(d): afficher "nanf"
          SINON SI isinf(d) et d > 0: afficher "+inff"
          SINON SI isinf(d) et d < 0: afficher "-inff"
          SINON:
              f = (float)d
              afficher f avec ".0f" si entier, sinon normal + "f"

    8. AFFICHER double(d):
          SI isnan(d): afficher "nan"
          SINON SI isinf(d) et d > 0: afficher "+inf"
          SINON SI isinf(d) et d < 0: afficher "-inf"
          SINON:
              afficher d avec ".0" si entier
```

---

## 11. CODE COMPLET C++98

### ScalarConverter.hpp

```cpp
#ifndef SCALARCONVERTER_HPP
# define SCALARCONVERTER_HPP

# include <string>

class ScalarConverter {
private:
    // Constructeurs privés → classe non-instanciable
    ScalarConverter();
    ScalarConverter(const ScalarConverter& other);
    ScalarConverter& operator=(const ScalarConverter& other);
    ~ScalarConverter();

public:
    // L'unique méthode publique, statique
    static void convert(const std::string& literal);
};

#endif
```

---

### ScalarConverter.cpp

```cpp
#include "ScalarConverter.hpp"

#include <iostream>   // std::cout
#include <string>     // std::string
#include <cmath>      // std::isnan, std::isinf
#include <climits>    // INT_MAX, INT_MIN, CHAR_MAX, CHAR_MIN
#include <cfloat>     // FLT_MAX, DBL_MAX
#include <cstdlib>    // std::strtod, std::strtof, std::strtol
#include <iomanip>    // std::fixed, std::setprecision
#include <cerrno>     // errno, ERANGE

// ─────────────────────────────────────────────
// Constructeurs privés — implémentés mais jamais appelés
// ─────────────────────────────────────────────
ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter&) {}
ScalarConverter& ScalarConverter::operator=(const ScalarConverter&) { return *this; }
ScalarConverter::~ScalarConverter() {}

// ─────────────────────────────────────────────
// Énumération des types
// ─────────────────────────────────────────────
enum LiteralType {
    PSEUDO_LITERAL,
    CHAR_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    INVALID_TYPE
};

// ─────────────────────────────────────────────
// Fonctions utilitaires de détection
// ─────────────────────────────────────────────

// Vérifie si la string est un pseudo-literal connu
static bool isPseudoLiteral(const std::string& s) {
    return (s == "nan"   || s == "nanf"  ||
            s == "+inf"  || s == "-inf"  ||
            s == "+inff" || s == "-inff");
}

// Vérifie si la string est un char littéral : 'x'
static bool isCharLiteral(const std::string& s) {
    return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
}

// Vérifie si la string est un nombre décimal valide (sans le 'f' final)
// Accepte : -4.2, +3.14, .5, 42.0
static bool isValidDecimalStr(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') i++;
    if (i >= s.length()) return false;
    bool hasDot = false;
    bool hasDigit = false;
    for (; i < s.length(); i++) {
        if (s[i] == '.') {
            if (hasDot) return false; // deux points → invalide
            hasDot = true;
        } else if (std::isdigit(s[i])) {
            hasDigit = true;
        } else {
            return false; // caractère non numérique
        }
    }
    return hasDigit && hasDot;
}

// Vérifie si la string est un entier valide : -42, 42, +42
static bool isValidIntStr(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') i++;
    if (i >= s.length()) return false;
    for (; i < s.length(); i++) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

// ─────────────────────────────────────────────
// Détection du type de la string
// ─────────────────────────────────────────────
static LiteralType detectType(const std::string& s) {
    if (isPseudoLiteral(s))
        return PSEUDO_LITERAL;
    if (isCharLiteral(s))
        return CHAR_TYPE;
    // Float : finit par 'f', le reste est un décimal
    if (s.length() > 1 && s[s.length() - 1] == 'f') {
        std::string withoutF = s.substr(0, s.length() - 1);
        if (isValidDecimalStr(withoutF))
            return FLOAT_TYPE;
    }
    if (isValidDecimalStr(s))
        return DOUBLE_TYPE;
    if (isValidIntStr(s))
        return INT_TYPE;
    return INVALID_TYPE;
}

// ─────────────────────────────────────────────
// Fonctions d'affichage
// ─────────────────────────────────────────────

// Affiche la représentation char
static void printChar(double d) {
    std::cout << "char: ";
    if (std::isnan(d) || std::isinf(d) || d < 0 || d > 127) {
        std::cout << "impossible" << std::endl;
    } else {
        char c = static_cast<char>(d);
        if (c < 32 || c == 127) {  // non-displayable ASCII
            std::cout << "Non displayable" << std::endl;
        } else {
            std::cout << "'" << c << "'" << std::endl;
        }
    }
}

// Affiche la représentation int
static void printInt(double d) {
    std::cout << "int: ";
    if (std::isnan(d) || std::isinf(d) ||
        d > static_cast<double>(INT_MAX) ||
        d < static_cast<double>(INT_MIN)) {
        std::cout << "impossible" << std::endl;
    } else {
        std::cout << static_cast<int>(d) << std::endl;
    }
}

// Affiche la représentation float
// Pour les entiers, force l'affichage ".0f"
static void printFloat(double d) {
    std::cout << "float: ";
    float f = static_cast<float>(d);
    if (std::isnan(f)) {
        std::cout << "nanf" << std::endl;
    } else if (std::isinf(f)) {
        std::cout << (f > 0 ? "+inff" : "-inff") << std::endl;
    } else {
        // Si c'est un entier, afficher avec ".0"
        if (f == static_cast<float>(static_cast<long>(f)))
            std::cout << std::fixed << std::setprecision(1) << f << "f" << std::endl;
        else
            std::cout << f << "f" << std::endl;
    }
}

// Affiche la représentation double
static void printDouble(double d) {
    std::cout << "double: ";
    if (std::isnan(d)) {
        std::cout << "nan" << std::endl;
    } else if (std::isinf(d)) {
        std::cout << (d > 0 ? "+inf" : "-inf") << std::endl;
    } else {
        if (d == static_cast<double>(static_cast<long>(d)))
            std::cout << std::fixed << std::setprecision(1) << d << std::endl;
        else
            std::cout << d << std::endl;
    }
}

// ─────────────────────────────────────────────
// Méthode principale : convert
// ─────────────────────────────────────────────
void ScalarConverter::convert(const std::string& literal) {
    LiteralType type = detectType(literal);
    double d = 0.0;  // Valeur pivot : tout est stocké en double

    // ── Parsing selon le type détecté ──────────────
    switch (type) {

        case PSEUDO_LITERAL:
            // Construire la valeur spéciale correspondante
            if (literal == "nan" || literal == "nanf")
                d = std::numeric_limits<double>::quiet_NaN();
            else if (literal == "+inf" || literal == "+inff")
                d = std::numeric_limits<double>::infinity();
            else if (literal == "-inf" || literal == "-inff")
                d = -std::numeric_limits<double>::infinity();
            break;

        case CHAR_TYPE:
            // 'a' → literal[1] = 'a'
            d = static_cast<double>(literal[1]);
            break;

        case INT_TYPE: {
            // strtol pour détecter overflow (errno = ERANGE)
            char* end;
            errno = 0;
            long l = std::strtol(literal.c_str(), &end, 10);
            if (errno == ERANGE || *end != '\0') {
                std::cerr << "Error: int overflow or invalid" << std::endl;
                return;
            }
            d = static_cast<double>(l);
            break;
        }

        case FLOAT_TYPE: {
            // strtof sur la string sans le 'f' final
            std::string withoutF = literal.substr(0, literal.length() - 1);
            char* end;
            float f = std::strtof(withoutF.c_str(), &end);
            if (*end != '\0') {
                std::cerr << "Error: invalid float" << std::endl;
                return;
            }
            d = static_cast<double>(f);
            break;
        }

        case DOUBLE_TYPE: {
            char* end;
            d = std::strtod(literal.c_str(), &end);
            if (*end != '\0') {
                std::cerr << "Error: invalid double" << std::endl;
                return;
            }
            break;
        }

        case INVALID_TYPE:
            std::cerr << "Error: invalid literal '" << literal << "'" << std::endl;
            return;
    }

    // ── Affichage des 4 types ──────────────────────
    printChar(d);
    printInt(d);
    printFloat(d);
    printDouble(d);
}
```

---

### main.cpp

```cpp
#include "ScalarConverter.hpp"
#include <iostream>

int main(int argc, char** argv) {
    // Vérification du nombre d'arguments
    if (argc != 2) {
        std::cerr << "Usage: ./convert <literal>" << std::endl;
        std::cerr << "Example: ./convert 42.0f" << std::endl;
        return 1;
    }

    // Délégation complète à la méthode statique
    ScalarConverter::convert(argv[1]);

    return 0;
}
```

---

## 12. TESTS COMPLETS

### 12.1 Tests basiques (exemples du sujet)

```bash
./convert 0
# char: Non displayable
# int: 0
# float: 0.0f
# double: 0.0

./convert nan
# char: impossible
# int: impossible
# float: nanf
# double: nan

./convert 42.0f
# char: '*'
# int: 42
# float: 42.0f
# double: 42.0
```

### 12.2 Tests pseudo-literals

```bash
./convert nanf
./convert +inf
./convert -inf
./convert +inff
./convert -inff
```

### 12.3 Tests char

```bash
./convert "'a'"    # char: 'a', int: 97, float: 97.0f, double: 97.0
./convert "' '"   # char: ' ', int: 32, float: 32.0f, double: 32.0
./convert "'\t'"  # Non displayable pour char (valeur 9)
```

### 12.4 Tests int

```bash
./convert 0
./convert 42
./convert -42
./convert 2147483647   # INT_MAX
./convert -2147483648  # INT_MIN
./convert 2147483648   # Overflow → comportement à définir
```

### 12.5 Tests float

```bash
./convert 0.0f
./convert 4.2f
./convert -4.2f
./convert 3.40282e+38f   # FLT_MAX
```

### 12.6 Tests double

```bash
./convert 0.0
./convert 4.2
./convert -4.2
./convert 1.7976931348623157e+308   # DBL_MAX
```

### 12.7 Tests edge cases

```bash
./convert 127        # char: DEL (127 = non displayable)
./convert 126        # char: '~' (dernier displayable)
./convert 32         # char: ' ' (premier displayable = espace)
./convert 31         # char: Non displayable
./convert -1         # char: impossible (négatif)
./convert 128        # char: impossible (>127)

./convert .5f        # float : 0.5f (pas de chiffre avant le point)
./convert -.5        # double : -0.5
```

### 12.8 Tests d'entrées invalides

```bash
./convert ""         # Error: invalid literal
./convert "abc"      # Error: invalid literal
./convert "1.2.3"    # Error: invalid literal
./convert "'ab'"     # Error: invalid literal (char avec 2 caractères)
./convert 1e999      # double overflow → +inf
```

### 12.9 Script de test automatisé

```bash
#!/bin/bash
EXEC="./convert"
PASS=0
FAIL=0

run_test() {
    INPUT="$1"
    EXPECTED="$2"
    RESULT=$($EXEC "$INPUT" 2>&1)
    if echo "$RESULT" | grep -q "$EXPECTED"; then
        echo "[PASS] $INPUT"
        PASS=$((PASS+1))
    else
        echo "[FAIL] $INPUT"
        echo "  Expected to contain: $EXPECTED"
        echo "  Got: $RESULT"
        FAIL=$((FAIL+1))
    fi
}

run_test "42.0f" "char: '*'"
run_test "42.0f" "int: 42"
run_test "42.0f" "float: 42.0f"
run_test "42.0f" "double: 42.0"

run_test "nan"   "int: impossible"
run_test "nan"   "float: nanf"
run_test "nan"   "double: nan"

run_test "0"     "char: Non displayable"
run_test "0"     "float: 0.0f"

run_test "+inf"  "char: impossible"
run_test "+inf"  "float: +inff"

echo ""
echo "Results: $PASS passed, $FAIL failed"
```

---

## Récapitulatif Final

```
┌────────────────────────────────────────────────────────────────────┐
│                    SCALARCONVERTER — RÉSUMÉ                        │
├────────────────────────────────────────────────────────────────────┤
│                                                                    │
│  STRING INPUT                                                      │
│      │                                                             │
│      ▼                                                             │
│  detectType()  →  PSEUDO / CHAR / INT / FLOAT / DOUBLE / INVALID  │
│      │                                                             │
│      ▼                                                             │
│  parseValue()  →  double d  (valeur pivot)                         │
│      │                                                             │
│      ▼                                                             │
│  ┌─────────────────────────────────────────────────────────────┐  │
│  │  printChar(d)   : vérif displayable + static_cast<char>     │  │
│  │  printInt(d)    : vérif overflow + static_cast<int>         │  │
│  │  printFloat(d)  : vérif nan/inf + static_cast<float>        │  │
│  │  printDouble(d) : vérif nan/inf + affichage direct          │  │
│  └─────────────────────────────────────────────────────────────┘  │
│                                                                    │
│  Règles d'or :                                                     │
│  • Toujours vérifier isnan/isinf AVANT de caster                  │
│  • Toujours vérifier les bornes AVANT de caster                   │
│  • Ordre de détection : pseudo → char → float → double → int      │
│  • Utiliser static_cast<> pour toutes les conversions             │
│                                                                    │
└────────────────────────────────────────────────────────────────────┘
```

---

*Cours rédigé pour le Module 06 de 42 School — C++ Casts*
*Bonne chance pour votre évaluation !*

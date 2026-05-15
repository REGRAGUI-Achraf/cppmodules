#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

const int LIT_PSEUDO = 0;
const int LIT_CHAR = 1;
const int LIT_INT = 2;
const int LIT_FLOAT = 3;
const int LIT_DOUBLE = 4;
const int LIT_INVALID = 5;

int detectLiteralType(const std::string& s);
bool isCharLiteral(const std::string& s);
bool isSingleChar(const std::string& s);
void printPseudo(const std::string& literal);
void printChar(double value);
void printInt(double value);
void printFloat(double value);
void printDouble(double value);
double parseLiteralToDouble(const std::string& literal, int type);

#endif

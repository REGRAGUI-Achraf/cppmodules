#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

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

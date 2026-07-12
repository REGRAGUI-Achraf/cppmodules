#include "Converter.hpp"
#include "Utils.hpp"

#include <iostream>

ScalarConverter::ScalarConverter() {}

ScalarConverter::ScalarConverter(const ScalarConverter& other) { (void)other; }

ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other) {
    (void)other;
    return *this;
}
ScalarConverter::~ScalarConverter() {}

void ScalarConverter::convert(const std::string& literal) {
    int type = detectLiteralType(literal);

    if (type == 0) { // PSEUDO
        printPseudo(literal);
        return;
    }

    if (type == 5) { // INVALID
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    double value = parseLiteralToDouble(literal, type);

    printChar(value);
    printInt(value);
    printFloat(value);
    printDouble(value);
}

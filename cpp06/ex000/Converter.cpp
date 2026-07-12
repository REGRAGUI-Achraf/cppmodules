#include "Converter.hpp"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

// Orthodox Canonical Form

ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter& other) { (void)other; }
ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other) {
    (void)other;
    return *this;
}
ScalarConverter::~ScalarConverter() {}

// Types 

enum LiteralType { PSEUDO, CHAR, INT, FLOAT, DOUBLE, INVALID };

// Detect

static bool isPseudoLiteral(const std::string& s) {
    return (s == "nan" || s == "nanf" || s == "+inf" || s == "-inf" ||
            s == "+inff" || s == "-inff");
}

static bool isCharLiteral(const std::string& s) {
    return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
}

static bool isSingleChar(const std::string& s) {
    return (s.length() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

static bool isValidDecimal(const std::string& s, bool requireDot) {
    if (s.empty())
        return false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-')
        i++;
    bool hasDigit = false;
    bool hasDot = false;
    for (; i < s.length(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(s[i]))) {
            hasDigit = true;
            continue;
        }
        if (s[i] == '.' && !hasDot) {
            hasDot = true;
            continue;
        }
        return false;
    }
    if (!hasDigit)
        return false;
    if (requireDot && !hasDot)
        return false;
    return true;
}

static bool isFloatLiteral(const std::string& s) {
    if (s.length() < 2)
        return false;
    char last = s[s.length() - 1];
    if (last != 'f' && last != 'F')
        return false;
    return isValidDecimal(s.substr(0, s.length() - 1), true);
}

static bool isDoubleLiteral(const std::string& s) {
    if (!isValidDecimal(s, true))
        return false;
    char last = s[s.length() - 1];
    return (last != 'f' && last != 'F');
}

static bool isIntLiteral(const std::string& s) {
    if (s.empty())
        return false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-')
        i++;
    if (i >= s.length())
        return false;
    for (; i < s.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    }
    return true;
}

static LiteralType detectLiteralType(const std::string& s) {
    if (isPseudoLiteral(s))
        return PSEUDO;
    if (isCharLiteral(s) || isSingleChar(s))
        return CHAR;
    if (isFloatLiteral(s))
        return FLOAT;
    if (isDoubleLiteral(s))
        return DOUBLE;
    if (isIntLiteral(s))
        return INT;
    return INVALID;
}

// Parsing

static double parseLiteralToDouble(const std::string& literal, LiteralType type) {
    if (type == CHAR)
        return isCharLiteral(literal) ? static_cast<double>(literal[1])
                                       : static_cast<double>(literal[0]);
    if (type == FLOAT)
        return std::strtod(literal.substr(0, literal.length() - 1).c_str(), 0);
    return std::strtod(literal.c_str(), 0); // INT ou DOUBLE
}

// Affichage 

static bool isIntegral(double value) {
    if (std::isnan(value) || std::isinf(value))
        return false;
    return std::fabs(value - std::floor(value)) < 1e-9;
}

static void printPseudo(const std::string& literal) {
    bool isNan = (literal == "nan" || literal == "nanf");
    bool isNeg = (!literal.empty() && literal[0] == '-');

    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;

    if (isNan) {
        std::cout << "float: nanf" << std::endl;
        std::cout << "double: nan" << std::endl;
    } else if (isNeg) {
        std::cout << "float: -inff" << std::endl;
        std::cout << "double: -inf" << std::endl;
    } else {
        std::cout << "float: +inff" << std::endl;
        std::cout << "double: +inf" << std::endl;
    }
}

static void printChar(double value) {
    if (std::isnan(value) || std::isinf(value) || value < 0.0 || value > 127.0) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    char c = static_cast<char>(value);
    if (!std::isprint(static_cast<unsigned char>(c)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << c << "'" << std::endl;
}

static void printInt(double value) {
    if (std::isnan(value) || std::isinf(value) ||
        value < static_cast<double>(std::numeric_limits<int>::min()) ||
        value > static_cast<double>(std::numeric_limits<int>::max())) {
        std::cout << "int: impossible" << std::endl;
        return;
    }
    std::cout << "int: " << static_cast<int>(value) << std::endl;
}

static void printFloat(double value) {
    std::cout << "float: ";
    if (std::isnan(value)) {
        std::cout << "nanf" << std::endl;
        return;
    }
    if (std::isinf(value)) {
        std::cout << (value < 0.0 ? "-inff" : "+inff") << std::endl;
        return;
    }
    float f = static_cast<float>(value);
    std::ostringstream oss;
    if (isIntegral(f))
        oss << std::fixed << std::setprecision(1) << f;
    else
        oss << std::setprecision(7) << f;
    std::cout << oss.str() << "f" << std::endl;
}

static void printDouble(double value) {
    std::cout << "double: ";
    if (std::isnan(value)) {
        std::cout << "nan" << std::endl;
        return;
    }
    if (std::isinf(value)) {
        std::cout << (value < 0.0 ? "-inf" : "+inf") << std::endl;
        return;
    }
    std::ostringstream oss;
    if (isIntegral(value))
        oss << std::fixed << std::setprecision(1) << value;
    else
        oss << std::setprecision(15) << value;
    std::cout << oss.str() << std::endl;
}

// input

void ScalarConverter::convert(const std::string& literal) {
    LiteralType type = detectLiteralType(literal);

    if (type == PSEUDO) {
        printPseudo(literal);
        return;
    }
    if (type == INVALID) {
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

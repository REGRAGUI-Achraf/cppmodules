#include "Utils.hpp"

#include <cstdlib>

static double parseNumber(const std::string& s) {
    return std::strtod(s.c_str(), 0);
}

double parseLiteralToDouble(const std::string& literal, int type) {
    if (type == 1) { // CHAR
        if (isCharLiteral(literal)) {
            return static_cast<double>(literal[1]);
        }
        return static_cast<double>(literal[0]);
    }
    if (type == 2) { // INT
        return parseNumber(literal);
    }
    if (type == 3) { // FLOAT
        return parseNumber(literal.substr(0, literal.length() - 1));
    }
    if (type == 4) { // DOUBLE
        return parseNumber(literal);
    }
    return 0.0;
}

#include "Utils.hpp"

#include <cstdlib>

static double parseNumber(const std::string& s) {
    return std::strtod(s.c_str(), 0);
}

double parseLiteralToDouble(const std::string& literal, int type) {
    if (type == LIT_CHAR) {
        if (isCharLiteral(literal)) {
            return static_cast<double>(literal[1]);
        }
        return static_cast<double>(literal[0]);
    }
    if (type == LIT_INT) {
        return parseNumber(literal);
    }
    if (type == LIT_FLOAT) {
        return parseNumber(literal.substr(0, literal.length() - 1));
    }
    if (type == LIT_DOUBLE) {
        return parseNumber(literal);
    }
    return 0.0;
}

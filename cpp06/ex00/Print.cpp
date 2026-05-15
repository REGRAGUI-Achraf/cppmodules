#include "Utils.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

static bool isIntegral(double value) {
    if (std::isnan(value) || std::isinf(value)) {
        return false;
    }
    double floored = std::floor(value);
    return std::fabs(value - floored) < 1e-9;
}

void printPseudo(const std::string& literal) {
    bool isNan = (literal == "nan" || literal == "nanf");
    bool isNeg = (!literal.empty() && literal[0] == '-');

    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;

    if (isNan) {
        std::cout << "float: nanf" << std::endl;
        std::cout << "double: nan" << std::endl;
        return;
    }

    if (isNeg) {
        std::cout << "float: -inff" << std::endl;
        std::cout << "double: -inf" << std::endl;
        return;
    }

    std::cout << "float: +inff" << std::endl;
    std::cout << "double: +inf" << std::endl;
}

void printChar(double value) {
    if (std::isnan(value) || std::isinf(value)) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    if (value < 0.0 || value > 127.0) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    char c = static_cast<char>(value);
    if (!std::isprint(static_cast<unsigned char>(c))) {
        std::cout << "char: Non displayable" << std::endl;
        return;
    }
    std::cout << "char: '" << c << "'" << std::endl;
}

void printInt(double value) {
    if (std::isnan(value) || std::isinf(value)) {
        std::cout << "int: impossible" << std::endl;
        return;
    }
    if (value < static_cast<double>(std::numeric_limits<int>::min()) ||
        value > static_cast<double>(std::numeric_limits<int>::max())) {
        std::cout << "int: impossible" << std::endl;
        return;
    }
    std::cout << "int: " << static_cast<int>(value) << std::endl;
}

void printFloat(double value) {
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
    if (isIntegral(f)) {
        oss << std::fixed << std::setprecision(1) << f;
    } else {
        oss << std::setprecision(7) << f;
    }
    std::cout << oss.str() << "f" << std::endl;
}

void printDouble(double value) {
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
    if (isIntegral(value)) {
        oss << std::fixed << std::setprecision(1) << value;
    } else {
        oss << std::setprecision(15) << value;
    }
    std::cout << oss.str() << std::endl;
}

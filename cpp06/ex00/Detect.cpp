#include "Utils.hpp"

#include <cctype>

static bool isPseudoLiteral(const std::string& s) {
    return (s == "nan" || s == "nanf" || s == "+inf" || s == "-inf" ||
            s == "+inff" || s == "-inff");
}

bool isCharLiteral(const std::string& s) {
    return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
}

bool isSingleChar(const std::string& s) {
    return (s.length() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

static bool isValidDecimal(const std::string& s, bool requireDot) {
    if (s.empty()) {
        return false;
    }
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') {
        i++;
    }
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
    if (!hasDigit) {
        return false;
    }
    if (requireDot && !hasDot) {
        return false;
    }
    return true;
}

static bool isFloatLiteral(const std::string& s) {
    if (s.length() < 2) {
        return false;
    }
    char last = s[s.length() - 1];
    if (last != 'f' && last != 'F') {
        return false;
    }
    std::string base = s.substr(0, s.length() - 1);
    return isValidDecimal(base, true);
}

static bool isDoubleLiteral(const std::string& s) {
    if (!isValidDecimal(s, true)) {
        return false;
    }
    if (!s.empty()) {
        char last = s[s.length() - 1];
        if (last == 'f' || last == 'F') {
            return false;
        }
    }
    return true;
}

static bool isIntLiteral(const std::string& s) {
    if (s.empty()) {
        return false;
    }
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') {
        i++;
    }
    if (i >= s.length()) {
        return false;
    }
    for (; i < s.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}

int detectLiteralType(const std::string& s) {
    if (isPseudoLiteral(s)) {
        return 0; // PSEUDO
    }
    if (isCharLiteral(s) || isSingleChar(s)) {
        return 1; // CHAR
    }
    if (isFloatLiteral(s)) {
        return 3; // FLOAT
    }
    if (isDoubleLiteral(s)) {
        return 4; // DOUBLE
    }
    if (isIntLiteral(s)) {
        return 2; // INT
    }
    return 5; // INVALID
}

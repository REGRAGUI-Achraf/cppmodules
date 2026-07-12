#include "bigint.hpp"
std::string bigint::addStrings(const std::string &a, const std::string &b) const
{
    std::string res;
    int carry = 0;
    int i = a.size() - 1;
    int j = b.size() - 1;
    while(i >= 0 || j >= 0 || carry)
    {
        int sum = carry;
        if(i >= 0)
            sum += a[i--] - '0';
        if(j >= 0)
            sum += b[j--] - '0';
        res += (sum % 10) + '0';
        carry = sum / 10;
    }
    std::reverse(res.begin(), res.end());
    return res;
}
unsigned long long bigint::stringToULL(const std::string &s) const
{
    unsigned long long res = 0;
    for(std::string::size_type i = 0; i < s.size(); ++i)
    {
        char c = s[i];
        if(!isdigit(c))
            throw std::invalid_argument("Invalid input string");
        res = res * 10 + (c - '0');
    }
    return res;
}
void bigint::remove_zeros()
{
    while(value.size() > 1 && value[0] == '0')
        value.erase(0, 1);
}
bigint::bigint() : value("0") {}
bigint::bigint(unsigned long long n) : value(std::to_string(n)) {}
bigint::bigint(const std::string& value) : value(value)
{
    remove_zeros();
}
bigint::bigint(const bigint& obj) : value(obj.value) {}
bigint& bigint::operator=(const bigint& obj)
{
    if(this != &obj)
        value = obj.value;
    return *this;
}
bigint bigint::operator+(const bigint& obj) const
{
    return bigint(addStrings(value, obj.value));
}
bigint bigint::operator+(unsigned long long n) const
{
    return *this + bigint(n);
}
bigint& bigint::operator+=(const bigint& obj)
{
    value = addStrings(value, obj.value);
    return *this;
}
bigint& bigint::operator+=(unsigned long long n)
{
    return *this += bigint(n);
}
bigint& bigint::operator++()
{
    *this += 1;
    return *this;
}
bigint bigint::operator++(int)
{
    bigint tmp(*this);
    ++(*this);
    return tmp;
}
bigint bigint::operator<<(int shift) const
{
    if(shift <= 0)
        return *this;
    return bigint(value + std::string(shift, '0'));
}
bigint bigint::operator>>(int shift) const
{
    if(shift <= 0 || shift >= static_cast<int>(value.size()))
        return bigint("0");
    return bigint(value.substr(0, value.size() - shift));
}
bigint& bigint::operator<<=(int shift)
{
    if(shift > 0)
        value += std::string(shift, '0');
    remove_zeros();
    return *this;
}
bigint& bigint::operator>>=(int shift)
{
    if(shift <= 0 || shift >= static_cast<int>(value.size()))
        value = "0";
    else
        value = value.substr(0, value.size() - shift);
    remove_zeros();
    return *this;
}
bigint bigint::operator<<(const bigint& shift) const
{
    return *this << stringToULL(shift.value);
}
bigint bigint::operator>>(const bigint& shift) const
{
    return *this >> stringToULL(shift.value);
}
bigint& bigint::operator>>=(const bigint& shift) 
{
    return *this >>= stringToULL(shift.value);
}
bigint& bigint::operator<<=(const bigint& shift)
{
    return *this <<= stringToULL(shift.value);
}
bool bigint::operator<(const bigint& obj) const
{
    if(value.size() != obj.value.size())
        return value.size() < obj.value.size();
    return value < obj.value;
}
bool bigint::operator>(const bigint& obj) const
{
    return obj < *this;
}
bool bigint::operator<=(const bigint& obj) const
{
    return !(obj < *this);
}
bool bigint::operator>=(const bigint& obj) const
{
    return !( *this < obj );
}
bool bigint::operator==(const bigint& obj) const
{
    return value == obj.value;
}
bool bigint::operator!=(const bigint& obj) const
{
    return !( *this == obj );
}
std::ostream& operator<<(std::ostream& out, const bigint& obj)
{
    out << obj.value;
    return out;
}
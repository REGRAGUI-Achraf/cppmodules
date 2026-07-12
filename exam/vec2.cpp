#include "vec2.hpp"
vect2::vect2() : x(0), y(0) {}
vect2::vect2(int x, int y) : x(x), y(y) {}
vect2::vect2(const vect2& obj) : x(obj.x), y(obj.y) {}
vect2& vect2::operator=(const vect2& obj) 
{
    if(this != &obj)
    {
        x = obj.x;
        y = obj.y;
    }
    return *this;
}
int& vect2::operator[](int i)
{
    if(i == 0)
        return x;
    else if(i == 1)
        return y;
    else
        throw std::out_of_range("Index out of bounds");
}
int vect2::operator[](int i) const
{
    if(i == 0)
        return x;
    else if(i == 1)
        return y;
    else
        throw std::out_of_range("Index out of bounds");
}
vect2 vect2::operator++(int)
{
    vect2 tmp = *this;
    x++;
    y++;
    return tmp;
}
vect2& vect2::operator++()
{
    x++;
    y++;
    return *this;
}
vect2 vect2::operator--(int)
{
    vect2 tmp = *this;
    x--;
    y--;
    return *this;
}
vect2& vect2::operator--()
{
    x--;
    y--;
    return *this;
}
vect2& vect2::operator+=(const vect2& obj)
{
    x += obj.x;
    y += obj.y;
    return *this;
}
vect2& vect2::operator-=(const vect2& obj)
{
    x -= obj.x;
    y -= obj.y;
    return *this;
}
vect2 vect2::operator+(const vect2& obj) const
{
    return vect2(x + obj.x, y + obj.y);
}
vect2 vect2::operator-(const vect2& obj)
{
    return vect2(x - obj.x, y - obj.y);
}
vect2 vect2::operator*(int n) const
{
    return vect2(x * n, y * n);
}
vect2 vect2::operator-()
{
    return vect2(-x, -y);
}
vect2& vect2::operator*=(int n)
{
    x *= n;
    y *= n;
    return *this;
}
bool vect2::operator==(const vect2& obj) const
{
    return x == obj.x && y == obj.y;
}
bool vect2::operator!=(const vect2& obj) const
{
    return !(*this == obj);
}
std::ostream& operator<<(std::ostream& os, const vect2& obj)
{
    os << "{" << obj[0] << ", " << obj[1] << "}";
    return os;
}
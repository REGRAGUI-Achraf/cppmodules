#include <iostream>

class vect2
{
    private:
        int x, y;
    public:
        vect2();
        vect2(int x,int y);
        vect2(const vect2 &obj);
        vect2& operator=(const vect2& obj);
        int& operator[](int i);
        int operator[](int i) const;
        vect2 operator++(int);
        vect2& operator++();
        vect2 operator--(int);
        vect2& operator--();
        vect2& operator+=(const vect2& obj);
        vect2& operator-=(const vect2& obj);
        vect2 operator+(const vect2& obj) const;
        vect2 operator-(const vect2& obj);
        vect2 operator-();
        vect2 operator*(int n) const;
        vect2& operator*=(int n);
        bool operator==(const vect2& obj) const;
        bool operator!=(const vect2& obj) const;
};
std::ostream &operator<<(std::ostream &os, const vect2 &obj);

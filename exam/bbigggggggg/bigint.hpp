#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <vector>
#include <climits>

using namespace std;
class bigint
{
    private:
		vector<int> num;
		//helper
		void trim(void);
		int convert(void) const;
    public:
		//OCF
        bigint();
        bigint(unsigned long long input);
        bigint(string input);
        bigint(const bigint& other);
        bigint& operator=(const bigint& other);
        ~bigint();
		//Acc
		vector<int> get_num(void) const;
		//operator
		bigint operator+(const bigint& other) const;
		bigint& operator+=(const bigint& other);
		bigint& operator++();
		bigint operator++(int);
		//comparison
		bool operator==(const bigint& other) const;
		bool operator!=(const bigint& other) const;
		bool operator>(const bigint& other) const;
		bool operator<(const bigint& other) const;
		bool operator>=(const bigint& other) const;
		bool operator<=(const bigint& other) const;
		//shift
		bigint operator<<(int n);
		bigint operator>>(int n);
		bigint& operator<<=(int n);
		bigint& operator>>=(int n);
		bigint operator<<(const bigint& n);
		bigint operator>>(const bigint& n);
		bigint& operator<<=(const bigint& n);
		bigint& operator>>=(const bigint& n);
};
//stream
ostream& operator<<(ostream& os, const bigint& bign);

#endif

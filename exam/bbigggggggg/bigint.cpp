#include "bigint.hpp"

//helper
void bigint::trim(void)
{
	while (this->num.size() > 1 && this->num.back() == 0)
		this->num.pop_back();
}

int bigint::convert(void) const
{
	long long res = 0;

	for (int i = 0; i < (int)this->num.size(); i++)
	{
		res = res * 10 + this->num[i];
		if(res >= INT_MAX)
			return INT_MAX;
	}
	return res;
}

//ocf
bigint::bigint()
{
	this->num.push_back(0);
}

bigint::bigint(unsigned long long input)
{
	if (input == 0)
	{
		this->num.push_back(0);
		return;
	}
	while (input)
	{
		this->num.push_back(input % 10);
		input /= 10;
	}
	trim();
}

bigint::bigint(string input)
{
	if(input.empty())
		this->num.push_back(0);
	for (int i = input.size() - 1; i >= 0; i--)
	{
		if(isdigit(input[i]))
			this->num.push_back(input[i] - '0');
	}
	trim();
}

bigint::bigint(const bigint& other)
{
	*this = other;
}

bigint& bigint::operator=(const bigint& other)
{
	if(*this != other)
		this->num = other.num;
	return *this;
}

bigint::~bigint(){}

//ACC
vector<int> bigint::get_num(void) const
{
	return this->num;
}

//operator
bigint bigint::operator+(const bigint& other) const
{
	bigint result;
	result.num.clear();
	int rest = 0, sum = 0;
	int size = max(this->num.size(), other.num.size());
	for (int i = 0; i < size; i++)
	{
		sum = rest;
		if(i < (int)this->num.size())
			sum += this->num[i];
		if(i < (int)other.num.size())
			sum += other.num[i];
		result.num.push_back(sum % 10);
		rest = sum / 10;
	}
	if (rest)
		result.num.push_back(rest);
	// result.trim();
	return result;
}

bigint& bigint::operator+=(const bigint& other)
{
	*this = *this + other;
	return *this;
}

bigint& bigint::operator++()
{
	*this = *this + bigint(1);
	return *this;
}

bigint bigint::operator++(int)
{
	bigint tmp(*this);
	++(*this);
	return tmp;
}

//comparison
bool bigint::operator==(const bigint& other) const
{
	return(this->num == other.num);
}

bool bigint::operator!=(const bigint& other) const
{
	return(this->num != other.num);
}

bool bigint::operator>(const bigint& other) const
{
	if(this->num.size() > other.num.size())
		return true;
	if(this->num.size() < other.num.size())
		return false;
	for (int i = (int)this->num.size() - 1; i >= 0; i--)
	{
		if(this->num[i] > other.num[i])
			return true;
		if(this->num[i] < other.num[i])
			return false;
	}
	return false;
}

bool bigint::operator<(const bigint& other) const
{
	return !(*this >= other);
}

bool bigint::operator>=(const bigint& other) const
{
	return (*this > other) || (*this == other);
}

bool bigint::operator<=(const bigint& other) const
{
	return !(*this > other);
}

//shift
bigint  bigint::operator<<(int n)
{
	bigint tmp(*this);
	if (n <= 0)
		return tmp;
	tmp.num.insert(tmp.num.begin(), n, 0);
	return tmp;
}

bigint bigint::operator>>(int n)
{
	bigint tmp(*this);
	if (n <= 0)
		return tmp;
	if ((int)tmp.num.size() > n)
		tmp.num.erase(tmp.num.begin(), tmp.num.begin() + n);
	else
	{
		tmp.num.clear();
		tmp.num.push_back(0);
	}
	return tmp;
}

bigint& bigint::operator<<=(int n)
{
	*this = *this << n;
	return *this;
}

bigint& bigint::operator>>=(int n)
{
	*this = *this >> n;
	return *this;
}

bigint bigint::operator<<(const bigint& n)
{
	return this->operator<<(n.convert());
}
bigint bigint::operator>>(const bigint& n)
{
	return this->operator>>(n.convert());
}
bigint& bigint::operator<<=(const bigint& n)
{
	return this->operator<<=(n.convert());
}
bigint& bigint::operator>>=(const bigint& n)
{
	return this->operator>>=(n.convert());
}

//stream
ostream& operator<<(ostream& os, const bigint& bign)
{
	vector<int> tmp = bign.get_num();
	for (int i = tmp.size() -1 ; i >= 0 ; i--)
	{
		os << tmp[i];
	}
	return os;
}

#pragma once

#include <ostream>

template < typename T >
class Money {
	T cash;
	T remote;
public:
	Money(T cash, T remote);
	Money operator + (Money other) const;
	friend std::ostream& operator << (std::ostream & os, const Money& money);
	T getCash() const;
	T getRemote() const;
	T total() const;
};
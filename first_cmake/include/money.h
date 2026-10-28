#pragma once

#include <ostream>

class Money {
	unsigned cash;
	unsigned remote;
public:
	Money(unsigned cash, unsigned remote);
	Money operator + (Money other) const;
	friend std::ostream& operator << (std::ostream & os, const Money& money);
};


#include "money.h"

Money::Money(unsigned cash, unsigned remote) : cash(cash), remote(remote) {
}

Money Money::operator+(Money other) const {
	return Money(cash + other.cash, remote + other.remote);
}

std::ostream& operator<<(std::ostream& os, const Money& money) {
	os << "{ Cash: " << money.cash << ", Remote: " << money.remote << " }";
	return os;
}




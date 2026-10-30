#include "money.h"

template<typename T>
Money<T>::Money(T cash, T remote) : cash(cash), remote(remote) {
}

template<typename T>
Money<T> Money<T>::operator+(Money<T> other) const {
	return Money<T>(cash + other.cash, remote + other.remote);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Money<T>& money) {
	os << "{ Cash: " << money.cash << ", Remote: " << money.remote << " }";
	return os;
}

template<typename T>
T Money<T>::getCash() const {
	return cash;
}

template<typename T>
T Money<T>::getRemote() const {
	return remote;
}

template<typename T>
T Money<T>::total() const {
	return cash + remote;
}

template class Money<int>;
template class Money<double>;
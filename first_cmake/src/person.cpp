#include "person.h"

Person::Person(std::string name) : name(name), money(0, 0) {
}

Person& Person::operator=(Money other) {
	money = other;
	return *this;
}

Money& Person::getMoney() {
	return money;
}

std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << "{ Name: " << person.name << ", Money: " << person.money << " }";
	return os;
}
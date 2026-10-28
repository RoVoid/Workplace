#include <iostream>

#include "person.h"

int main() {
	Person p1("Olga"), p2("Pete");

	p1 = { 10, 2 };
	p2 = { 4, 35 };

	std::cout << p1 << '\n';
	std::cout << p2 << '\n';

	std::cout << std::endl;

	p1 = p1.getMoney() + p2.getMoney();

	std::cout << p1 << '\n';
	std::cout << p2 << std::endl;

	return 0;
}

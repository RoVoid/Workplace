#pragma once

#include <string>

#include "money.h"

class Person {
	std::string name;
	Money<double> money;
public:
	Person(std::string name);
	Person& operator = (Money<double> other);
	Money<double>& getMoney();
	friend std::ostream& operator << (std::ostream & os, const Person& person);
};


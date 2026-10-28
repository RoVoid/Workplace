#pragma once

#include <string>

#include "money.h"

class Person {
	std::string name;
	Money money;
public:
	Person(std::string name);
	Person& operator = (Money other);
	Money& getMoney();
	friend std::ostream& operator << (std::ostream & os, const Person& person);
};


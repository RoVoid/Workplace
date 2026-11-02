#pragma once

#include <iostream>

class Workable {
protected:
	int kpd;
public:
	Workable(int kpd);
	~Workable();
	Workable(const Workable& other);
	Workable& operator = (const Workable& other);
	void work();
};

class Human: Workable {
public:
	Human(int kpd);
	void work();
};

class Robot: Workable {
public:
	Robot(int kpd);
	void work();
};
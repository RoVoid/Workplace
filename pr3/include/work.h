#pragma once

#include <iostream>

class Workable {
protected:
	int kpd;
public:
	Workable(int kpd);
	Workable(const Workable& other);
	Workable& operator = (const Workable& other);
	virtual ~Workable();
	virtual void work() = 0;
};

class Human: public Workable {
public:
	Human(int kpd);
	Human(const Human& other);
	~Human();
	void work() override;
};

class Robot: public Workable {
public:
	Robot(int kpd);
	Robot(const Robot& other);
	~Robot();
	void work() override;
};
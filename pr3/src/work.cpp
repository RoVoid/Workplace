#include "work.h"

using std::cout;
using std::endl;

unsigned humans = 0, robots = 0;

Workable::Workable(int kpd) : kpd(kpd) {
	cout << "Hire with kpd = " << kpd << endl;
}

Workable::~Workable() {
	cout << "Free with kpd = " << kpd << endl;
}

Workable::Workable(const Workable& other) {
	kpd = other.kpd;
	cout << "Copy with kpd = " << kpd << endl;
}

Workable& Workable::operator=(const Workable& other) {
	kpd = other.kpd;
	cout << "Equals with kpd = " << kpd << endl;
	return *this;
}

Human::Human(int kpd) : Workable(kpd) {
	cout << "Hire human#" << humans++ << " with kpd = " << kpd << endl;
}

Human::Human(const Human& other) : Workable(other) {
	++humans;
	cout << "Copy human, total: " << humans << endl;
}

Human::~Human() {
	cout << "Free human#" << --humans << " with kpd = " << kpd << endl;
}


void Human::work() {
	cout << "I can work with kpd = " << kpd << ", but not... Use Robot! :D" << endl;
}

Robot::Robot(int kpd) : Workable(kpd) {
	cout << "Hire robot#" << robots++ << " with kpd = " << kpd << endl;
}

Robot::Robot(const Robot& other) : Workable(other) {
	++robots;
	cout << "Copy robot, total: " << robots << endl;
}

Robot::~Robot() {
	cout << "Free robot#" << --robots << " with kpd = " << kpd << endl;
}

void Robot::work() {
	cout << "Work with kpd = " << kpd << endl;
}
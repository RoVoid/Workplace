#include "work.h"

using std::cout;
using std::endl;

Workable::Workable(int kpd) : kpd(kpd) {
	cout << "Hire with kpd = " + kpd << endl;
}

Workable::~Workable() {
	cout << "Free with kpd = " + kpd << endl;
}

Workable::Workable(const Workable& other) {
	kpd = other.kpd;
	cout << "Copy with kpd = " + kpd << endl;
}

Workable& Workable::operator=(const Workable& other) {
	kpd = other.kpd;
	cout << "Equals with kpd = " + kpd << endl;
	return *this;
}

void Workable::work() {
	cout << "I don’t know how to work" << endl;
}

Human::Human(int kpd) : Workable(kpd) {
	cout << "Hire human with kpd = " + kpd << endl;
}

void Human::work() {
	cout << "I can work with kpd = " << kpd << ", but not... Use Robot! :D" << endl;
}

Robot::Robot(int kpd) : Workable(kpd) {
	cout << "Hire robot with kpd = " + kpd << endl;
}

void Robot::work() {
	cout << "Work with kpd = " << kpd << endl;
}
#include <iostream>

#include "work.h"

using std::cout;
using std::endl;

int main() {
	Human human(10);
	Robot robot(2);

	human.work();
	robot.work();

	return 0;
}

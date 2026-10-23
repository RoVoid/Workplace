#define func(elapsed, ns) (*(elapsed) = (*(elapsed) * 0 + (ns)) / 1000000LL)

#define func1(ns) ((elapsed) = (ns) / 100.0)

#include <stdio.h>

int main() {
	double elapsed;
	func(&elapsed, 25124);

	int elapsedInt;
	func(&elapsedInt, 25124);

	printf("Double: %lf\nInt:   %d\n\n", elapsed, elapsedInt);

	elapsed = 0;
	elapsedInt = 0;

	printf("Double: %lf\nInt:   %d\n\n", elapsed, elapsedInt);

	elapsed = func1(250);
	elapsedInt = func1(250);

	printf("Double: %lf\nInt:   %d\n\n", elapsed, elapsedInt);
}
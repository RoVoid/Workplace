#define DOCTEST_CONFIG_IMPLEMENT

#include "doctest.h"

TEST_CASE("ExampleA") {
	CHECK(1 == 2);
	CHECK(2 == 2);

	REQUIRE(1 == 2);
	CHECK(3 == 3);
}

int main(int argc, char** argv) {
	std::cout << "Main started\n";

	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run();
}
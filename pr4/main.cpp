#include <iostream>
#include <boost/program_options.hpp>
#include "work.h"

using std::cout;
using std::endl;

namespace opt = boost::program_options;

int main(int argc, char** argv) {

	opt::options_description desc("All options");

	int human_kpd = 0, robot_kpd = 0;

	desc.add_options()
	("human,h", opt::value<int>(&human_kpd)->default_value(2), "kpd for human")
	("robot,r", opt::value<int>(&robot_kpd)->required(), "kpd for robot")
	("help", "produce help message");

	opt::variables_map vm;

	opt::store(opt::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 0;
	}

	opt::notify(vm);

	Human human(human_kpd);
	Robot robot(robot_kpd);

	human.work();
	robot.work();

	return 0;
}
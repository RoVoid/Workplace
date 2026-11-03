#include <iostream>
#include <memory>
#include <vector>

#include "work.h"

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

int main() {

	Human human(10);
	Robot robot(2);

	human.work();
	robot.work();

	cout << endl;

	Human human1 = human;

	human1.work();

	cout << endl;


	unique_ptr<Workable> worker1 = make_unique<Human>(15);
	unique_ptr<Workable> worker2 = make_unique<Robot>(5);

	worker1->work();
	worker2->work();

	cout << endl;

	unique_ptr<Workable> worker3 = std::move(worker2);

	if (!worker2) cout << "worker2 is nullptr\n";

	worker3->work();

	cout << endl;

	std::vector<unique_ptr<Workable> > workers;

	workers.push_back(make_unique<Human>(20));
	workers.push_back(make_unique<Robot>(7));
	workers.push_back(make_unique<Robot>(12));

	for (const auto& worker : workers) worker->work();

	cout << endl;

	shared_ptr<Workable> sharedWorker = make_shared<Robot>(100);

	{
		shared_ptr<Workable> copy1 = sharedWorker;
		shared_ptr<Workable> copy2 = sharedWorker;

		cout << "Owners: " << sharedWorker.use_count() << endl;

		copy1->work();
	}

	cout << "Owners after scope: " << sharedWorker.use_count() << endl;

	sharedWorker->work();

	return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <utility> // для pair

using namespace std;

const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

template <typename T>
bool assertEqual(const T& actual, const T& expected) {
    if (actual == expected) {
        cout << GREEN << "[PASSED] " << CYAN << expected << RESET << endl;
        return true;
    }
    else {
        cout << RED << "[FAILED] " << CYAN << expected << RESET
            << " != " << YELLOW << actual << RESET << endl;
        return false;
    }
}

bool func0(int n) {
    bool result = true;
    int N1;
    N1 = n;
    int len = N1 ? 0 : 1;
    while (N1) { len++, N1 /= 10; }

    int test[len], test1[len];

    for (int i = 0; i < len; i++) {
        test1[len - i - 1] = test[i] = n % 10;
        n /= 10;
    }
    for (int i = 0; i < len; i++) {
        if (test1[i] == test[i]) continue;
        result = false;
        break;
    }

    return result;
}

void runTestsForTask0() {
    vector<pair<int, bool>> tests = {
        {10245, false}, {10201, true}, {1, true}, {49, false},
        {102201, true}, {1022001, false}, {6245426, true}, {6245462, false},
        {598657214, false}, {1000220001, true}, {624486426, false}, {262575262, true}
    };

    for (const auto& test : tests) {
        //cout << RESET << test.first << "\n ";
        assertEqual(func0(test.first), test.second);
    }
}

int main() {
    int taskNumber;
    cin >> taskNumber;
    switch (taskNumber) {
    case 0:
        runTestsForTask0();
        break;
    default:
        cout << RED << "Invalid task number" << RESET << endl;
        break;
    }

    return 0;
}
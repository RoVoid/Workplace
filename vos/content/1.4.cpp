// Поиск

#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    int x, a;

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input || !output) return 2;

    input >> x;
    while (input >> a) {
        if (a == x) {
            output << 1;
            return 0;
        }
    }

    output << 0;
    return 0;
}

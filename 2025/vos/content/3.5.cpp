#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input || !output) return 2;

    unsigned long long n, i = 1ull << 63, j = 63;
    input >> n;
    if (n == 0) return 3;

    for (; !(n & i); i >>= 1, --j);
    output << j << std::flush;
    return 0;
}

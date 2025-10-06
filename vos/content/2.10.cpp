#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input || !output) return 2;

    unsigned n;
    input >> n;
    if (n < 0) return 3;

    output << n + 1 << " ";

    unsigned long long c = 1;
    for (unsigned k = 0; k <= n; ++k) {
        output << c << " ";
        c = c * (n - k) / (k + 1);
    }

    output << std::flush;
    return 0;
}

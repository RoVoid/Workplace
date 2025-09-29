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

    unsigned long long* a = new unsigned long long[n + 1];
    a[0] = 1;

    for (unsigned i = 1; i <= n; ++i) {
        for (int j = i; j > 0; --j) a[j] = a[j] + a[j - 1];
    }

    output << n + 1 << " ";
    for (unsigned i = 0; i <= n; ++i) output << a[i] << " ";
    output << std::flush;

    delete[] a;
    return 0;
}

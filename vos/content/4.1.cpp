#include <fstream>
#include <iostream>
#include <ostream>
#include <utility>

using std::ifstream;
using std::ofstream;

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input || !output) return 2;

    long long n, *a, i;
    input >> n;
    if (n <= 0) return 3;

    a = new long long[n];
    for (i = 0; i < n; ++i)
        if (!(input >> a[i])) {
            delete[] a;
            return 4;
        }

    for (i = 0; i < n - 1; ++i) {
        if (a[i] <= a[i + 1]) continue;
        std::swap(a[i], a[i + 1]);
        i = -1;
    }

    output << n << ' ';
    for (i = 0; i < n; ++i) output << a[i] << ' ';
    output << std::flush;

    delete[] a;
    return 0;
}

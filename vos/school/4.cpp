#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
    int n, m;
    long long k, s, p = 0;

    cin >> m >> n >> k;

    --k;
    s = m * n;

    while (p < k) {
        if (n % k < m % k) {
            --m;
            p += n;
        } else {
            --n;
            p += m;
        }
    }

    cout << s - p << endl;

    return 0;
}
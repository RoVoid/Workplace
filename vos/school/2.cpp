#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::swap;

int main() {
    int n, a, b, x, y;
    cin >> n >> a >> x >> b >> y;

    if (x < y) {
        swap(a, b);
        swap(x, y);
    }

    for (int i = x; i < n;) {
        if (a > 0) {
            cout << i << ' ' << x << endl;
            --a;
            i += 2 * x + 1;
            if (a == 0 && b > 0) i -= x - y;
        } else if (b > 0) {
            cout << i << ' ' << y << endl;
            --b;
            i += 2 * y + 1;
        } else {
            cout << -1 << endl;
            return 0;
        }
    }

    return 0;
}

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main() {
    int e, n;
    cin >> e >> n;

    vector<int> a(n + 2, 0);
    for (int i = 1; i <= n; i++) cin >> a[i];

    int p = 0;

    for (int i = 1; i <= n + 1; i++) {
        if (a[i] < 0) continue;
        int move = i - p;
        if (move > e) {
            cout << -1 << endl;
            return 0;
        }
        e += a[i] - move;
        p = i;
    }

    cout << (p != n + 1 ? -1 : e) << endl;

    return 0;
}
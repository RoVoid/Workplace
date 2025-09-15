#include <iostream>
#include <vector>
using namespace std;

int main() {
    int e, n;
    cin >> e >> n;

    vector<int> a(n + 2, 0);
    for (int i = 1; i <= n; i++) cin >> a[i];

    int last = 0;

    for (int i = 1; i <= n; i++) {
        int need = i - last;
        if (e < need) {
            cout << -1 << endl;
            return 0;
        }
        e += -need + a[i];
        last = i;
        if (e < 0) {
            cout << -1 << endl;
            return 0;
        }
    }

    cout << (e < 1 ? -1 : e) << endl;

    return 0;
}

// не решено